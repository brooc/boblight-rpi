/*
 * boblight
 * Copyright (C) Bob  2009 
 * 
 * boblight is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * boblight is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "util/inclstdint.h"

#include "grabber-openmax.h"

#include <string.h>
#include <assert.h>

#include "util/misc.h"

#define BOBLIGHT_DLOPEN_EXTERN
#include "lib/boblight.h"

using namespace std;

CGrabberOpenmax::CGrabberOpenmax(void* boblight, volatile bool& stop, bool sync) : m_stop(stop), m_timer(&stop)
{
	int ret;

	bcm_host_init();

	m_boblight = boblight;
	m_debug = false;
	m_sync = sync;

	type = VC_IMAGE_RGB888;

    display  = vc_dispmanx_display_open(0);

    ret = vc_dispmanx_display_get_info(display, &info);
    assert(ret == 0);

	pitch = ALIGN_UP(info.width*3, 32);

	image = new char[pitch * info.height];

	resource = vc_dispmanx_resource_create(type,
										   info.width,
										   info.height,
										   &vc_image_ptr );
	assert(resource);
}

CGrabberOpenmax::~CGrabberOpenmax()
{
	int ret;

	free(image);

    ret = vc_dispmanx_resource_delete(resource);
    assert( ret == 0 );
    ret = vc_dispmanx_display_close(display);
    assert( ret == 0 );

    bcm_host_deinit();
}

bool CGrabberOpenmax::Setup()
{
  if (m_interval > 0.0) //set up timer
  {
    m_timer.SetInterval(Round64(m_interval * 1000000.0));
  }

  m_error.clear();

  return ExtendedSetup(); //run stuff from derived classes
}

bool CGrabberOpenmax::ExtendedSetup()
{
  if (!CheckExtensions())
    return false;

  return true;
}

bool CGrabberOpenmax::CheckExtensions()
{
  return true;
}

bool CGrabberOpenmax::Run()
{
  int rgb[3];
  int downsampled_width, downsampled_height;
  VC_RECT_T rectangle;
  char* image_ptr;

  boblight_setscanrange(m_boblight, m_size, m_size);

  downsampled_width = info.width * 3 / m_size;
  downsampled_height = info.height / m_size;

  while(!m_stop)
  {
	  vc_dispmanx_snapshot(display,resource, VC_IMAGE_ROT0);

	  vc_dispmanx_rect_set(&rectangle, 0, 0, info.width, info.height);

	  vc_dispmanx_resource_read_data(resource, &rectangle, image, pitch);

	  //read out the pixels
	  for (int y = 0; y < m_size && !m_stop; y++)
	  {
		  image_ptr = (char *)image + pitch * y * downsampled_height;

		  for (int x = 0; x < m_size && !m_stop; x++)
		  {
			  rgb[0] = *(image_ptr + downsampled_width * x);//(info.width) * 3 * x/m_size);
			  rgb[1] = *(image_ptr + downsampled_width * x + 1);//(info.width) * 3 * x/m_size + 1);
			  rgb[2] = *(image_ptr + downsampled_width * x + 2);//(info.width) * 3 * x/m_size + 2);

			  boblight_addpixelxy(m_boblight, x, y, rgb);
		  }
	  }


	  //send pixeldata to boblight
	  if (!boblight_sendrgb(m_boblight, m_sync, NULL))
	  {
		  m_error = boblight_geterror(m_boblight);
		  return true; //recoverable error
	  }

	  //when in debug mode, put the captured image on the debug window
	  if (m_debug)
	  {
		  printf("Debug not supproted!\n");
		  m_debug = false;
	  }

	  if (!Wait())
	  {
		  return false; //unrecoverable error
	  }
  }

  m_error.clear();

  return true;
}

bool CGrabberOpenmax::Wait()
{
  if (m_interval > 0.0) //wait for timer
  {
    m_timer.Wait();
  }
  return true;
}

