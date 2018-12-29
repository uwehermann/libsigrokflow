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

void init()
{
}

LegacyCaptureDevice::LegacyCaptureDevice(shared_ptr<sigrok::Device> device) :
	_device(device)
{
}

shared_ptr<sigrok::Device> LegacyCaptureDevice::libsigrok_device()
{
	return _device;
}

}
