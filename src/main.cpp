/*
 * This file is part of the libsigrokflow project.
 *
 * Copyright (C) 2018 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <libsigrokflow/libsigrokflow.hpp>

#include <iostream>

namespace Srf
{

using namespace std;
using namespace std::placeholders;

void init()
{
	Gst::Plugin::register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
			"sigrok_legacy_capture_device",
			"Wrapper for capture devices using legacy libsigrok APIs",
			sigc::ptr_fun(&LegacyCaptureDevice::register_element),
			"0.01", "GPL", "sigrok", "libsigrokflow", "http://sigrok.org");
}

GstBlock::GstBlock(GstElement *gobj) :
	Gst::Element(gobj)
{
}

Device::Device(GstElement *gobj) :
	GstBlock(gobj)
{
}

CaptureDevice::CaptureDevice(GstElement *gobj) :
	Device(gobj)
{
}

void LegacyCaptureDevice::class_init(Gst::ElementClass<LegacyCaptureDevice> *klass)
{
	klass->set_metadata("sigrok legacy capture device",
			"Source", "Wrapper for capture devices using legacy libsigrok APIs",
			"Martin Ling");

	klass->add_pad_template(Gst::PadTemplate::create(
			"src",
			Gst::PAD_SRC,
			Gst::PAD_ALWAYS,
			Gst::Caps::create_any()));
}

bool LegacyCaptureDevice::register_element(Glib::RefPtr<Gst::Plugin> plugin)
{
	Gst::ElementFactory::register_element(plugin, "sigrok_legacy_capture_device",
			0, Gst::register_mm_type<LegacyCaptureDevice>(
				"sigrok_legacy_capture_device"));
	return true;
}

LegacyCaptureDevice::LegacyCaptureDevice(GstElement *gobj) :
	Glib::ObjectBase(typeid(LegacyCaptureDevice)),
	CaptureDevice(gobj)
{
	add_pad(_src_pad = Gst::Pad::create(get_pad_template("src"), "src"));
}

Glib::RefPtr<LegacyCaptureDevice>LegacyCaptureDevice::create(
	shared_ptr<sigrok::HardwareDevice> libsigrok_device)
{
	auto element = Gst::ElementFactory::create_element("sigrok_legacy_capture_device");
	if (!element)
		throw runtime_error("Failed to create element - plugin not registered?");
	auto device = Glib::RefPtr<LegacyCaptureDevice>::cast_static(element);
	device->_libsigrok_device = libsigrok_device;
	return device;
}

shared_ptr<sigrok::HardwareDevice> LegacyCaptureDevice::libsigrok_device()
{
	return _libsigrok_device;
}

Gst::StateChangeReturn LegacyCaptureDevice::change_state_vfunc(Gst::StateChange transition)
{
	switch (transition)
	{
		case Gst::STATE_CHANGE_READY_TO_PAUSED:
			return Gst::StateChangeReturn::STATE_CHANGE_NO_PREROLL;
		case Gst::STATE_CHANGE_PAUSED_TO_PLAYING:
			_task = Gst::Task::create(std::bind(&LegacyCaptureDevice::_run, this));
			_task->set_lock(_mutex);
			_src_pad->set_active(true);
			_task->start();
			return Gst::STATE_CHANGE_SUCCESS;
		default:
			return Gst::STATE_CHANGE_SUCCESS;
	}
}

void LegacyCaptureDevice::_datafeed_callback(
	shared_ptr<sigrok::Device> device,
	shared_ptr<sigrok::Packet> packet)
{
	(void) device;
	switch (packet->type()->id()) {
		case SR_DF_LOGIC:
		{
			auto logic = static_pointer_cast<sigrok::Logic>(packet->payload());
			auto mem = Gst::Memory::create(
					Gst::MEMORY_FLAG_READONLY,
					logic->data_pointer(),
					logic->data_length(),
					0,
					logic->data_length());
			auto buf = Gst::Buffer::create();
			buf->append_memory(move(mem));
			_src_pad->push(move(buf));
			break;
		}
		case SR_DF_END:
			_session->stop();
			_src_pad->push_event(Gst::EventEos::create());
			break;
		default:
			break;
	}
}

void LegacyCaptureDevice::_run()
{
	_session = _libsigrok_device->driver()->parent()->create_session();
	_session->add_device(_libsigrok_device);
	_session->add_datafeed_callback(bind(&LegacyCaptureDevice::_datafeed_callback, this, _1, _2));
	_session->start();
	_session->run();
	_task->stop();
}

}
