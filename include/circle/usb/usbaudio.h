//
// usbaudio.h
//
// Ported from the USPi driver which is:
// 	Copyright (C) 2016  J. Otto <joshua.t.otto@gmail.com>
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2017-2022  R. Stange <rsta2@o2online.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _circle_usb_usbaudio_h
#define _circle_usb_usbaudio_h

#include <circle/macros.h>

// Audio class endpoint descriptor
struct TUSBAudioEndpointDescriptor
{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bEndpointAddress;
	unsigned char	bmAttributes;
	unsigned short	wMaxPacketSize;
	unsigned char	bInterval;
	unsigned char	bRefresh;
	unsigned char	bSynchAddress;
}
PACKED;

// MIDI-streaming class-specific endpoint descriptor
struct TUSBMIDIStreamingEndpointDescriptor
{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubType;
	unsigned char	bNumEmbMIDIJack;
	unsigned char	bAssocJackIDs[];
}
PACKED;

// Audio class type I format type descriptor
struct TUSBAudioTypeIFormatTypeDescriptor
{
	unsigned char	bLength;
	unsigned char	bDescriptorType;
	unsigned char	bDescriptorSubtype;
	unsigned char	bFormatType;
	unsigned char	bNrChannels;
	unsigned char	bSubframeSize;
	unsigned char	bBitResolution;
	unsigned char	bSamFreqType;
	unsigned char	tSamFreq[][3];
}
PACKED;

#endif
