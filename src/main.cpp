/*
 * This file is part of the libsigrokflow project.
 *
 * Copyright (C) 2018 Martin Ling <martin-sigrok@earth.li>
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
#include <gstreamermm.h>
#include <libsigrokflow/main.hpp>

namespace Srf
{

Sink::Sink(GstBaseSink *gobj) :
	Gst::BaseSink(gobj)
{
}

Device::Device(GstElement *gobj) :
	Gst::Element(gobj)
{
}

CaptureDevice::CaptureDevice(GstElement *gobj) :
	Device(gobj)
{
}

LegacyLogicChannelEncoding::LegacyLogicChannelEncoding(uint32_t bit_index) :
	bit_index_(bit_index)
{
}

Channel::Channel(string name) :
	name_(name)
{
}

LogicChannel::LogicChannel(string name) :
	Channel(name)
{
}

Pad::Pad(const Glib::RefPtr<const Gst::PadTemplate>& pad_template, const Glib::ustring& name) :
	Gst::Pad(pad_template, name)
{
}

Glib::RefPtr<Pad> Pad::create(const Glib::RefPtr<const Gst::PadTemplate>& pad_template,
		const Glib::ustring& name)
{
	// Create a new Srf::Pad, wrap it in GLib::RefPtr and return that.
	return Glib::RefPtr<Pad>(new Pad(pad_template, name));
}

}
