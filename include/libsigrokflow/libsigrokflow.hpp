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

#ifndef LIBSIGROKFLOW_LIBSIGROKFLOW_HPP
#define LIBSIGROKFLOW_LIBSIGROKFLOW_HPP

#include <gstreamermm.h>
#include <libsigrokcxx/libsigrokcxx.hpp>

namespace Srf
{

using namespace std;

void init();

class Block
{
        /* Config API etc goes here */
};

class GstBlock :
        public Gst::Element
{
        /* Operations specific to sigrok GStreamer blocks go here. */
};

class Device :
        public GstBlock
{
        /* Operations specific to hardware devices go here */
};

class CaptureDevice :
        public Device
{
        /* Operations specific to capture (source) devices go here */
protected :
        CaptureDevice();
};

class LegacyCaptureDevice :
        public CaptureDevice
{
public:
        /* Construct from libsigrok device object */
        LegacyCaptureDevice(shared_ptr<sigrok::Device>);

        /* Retrieve libsigrok device object */
        shared_ptr<sigrok::Device> libsigrok_device();
private:
        shared_ptr<sigrok::Device> _device;
};


}
#endif
