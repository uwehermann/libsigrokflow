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

#ifndef LIBSIGROKFLOW_MAIN_HPP
#define LIBSIGROKFLOW_MAIN_HPP

#include <gstreamermm.h>

namespace Srf
{

using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

class Datastream;

class Block
{
	/* Config API etc. goes here. */
};

class Sink :
	public Gst::BaseSink
{
protected:
	explicit Sink(GstBaseSink *gobj);
};

class Device :
	public Gst::Element
{
	/* Operations specific to hardware devices go here. */
protected:
	explicit Device(GstElement *gobj);
};

class CaptureDevice :
	public Device
{
	/* Operations specific to capture (source) devices go here. */
protected:
	explicit CaptureDevice(GstElement *gobj);
};

class Pad :
	public Gst::Pad
{
private:
	Pad(const Glib::RefPtr<const Gst::PadTemplate>& pad_template, const Glib::ustring& name);

public:
	static Glib::RefPtr<Pad> create(const Glib::RefPtr<const Gst::PadTemplate>& pad_template, const Glib::ustring& name);

	shared_ptr<Datastream> datastream_;
};

class ChannelEncoding
{
};

class LegacyLogicChannelEncoding :
	public ChannelEncoding
{
public:
	LegacyLogicChannelEncoding(uint32_t bit_index);

private:
	uint32_t bit_index_;
};

class LegacyAnalogChannelEncoding :
	public ChannelEncoding
{
};

class Channel
{
public:
	Channel(string name);

	string name_;
};

class LogicChannel :
	public Channel
{
public:
	LogicChannel(string name);
};

class AnalogChannel :
	public Channel
{
};

class DatastreamEncoding
{
};

class LegacyLogicDatastreamEncoding :
	public DatastreamEncoding
{
public:
	uint32_t unitsize_;
};

class Datastream
{
public:
	map<shared_ptr<Channel>, shared_ptr<ChannelEncoding>> channels;

	shared_ptr<DatastreamEncoding> encoding;
};

}
#endif
