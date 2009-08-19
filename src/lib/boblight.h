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

#ifndef CBOBLIGHT
#define CBOBLIGHT

#include <string>
#include <vector>

#include "util/tcpsocket.h"
#include "util/messagequeue.h"
#include "util/clock.h"

#include "libboblight.h"

class CLight
{
  public:
    CLight();

    std::string SetOption(const char* option);
    std::string GetOption(const char* option, std::string& output);
    
    std::string m_name;
    float       m_speed;
    bool        m_autospeed;
    float       m_autospeedvalue;
    
    bool        m_interpolation;
    bool        m_use;

    float       m_value;
    float       m_saturation;
    float       m_valuerange[2];
    int         m_threshold;

    int         m_rgbd[4];
    float       m_prevrgb[3];
    void        GetRGB(float* rgb);

    float       m_hscan[2];
    float       m_vscan[2];
    int         m_width;
    int         m_height;
    int         m_hscanscaled[2];
    int         m_vscanscaled[2];
};  

class CBoblight
{
  public:
    CBoblight();
    
    int         Connect(const char* address, int port, int usectimeout);
    const char* GetError()                    { return m_error.c_str(); }

    int         GetNrLights()                 { return m_lights.size(); }
    const char* GetLightName    (int lightnr);

    int         SetPriority     (int priority);
    void        SetScanRange    (int width,   int height);

    int         AddPixel(int lightnr, int* rgb);
    void        AddPixel(int* rgb, int x, int y);

    int         SendRGB();
    int         Ping();

    int         GetNrOptions();
    const char* GetOptionDescription(int option);
    int         SetOption(int lightnr, const char* option);
    int         GetOption(int lightnr, const char* option, const char** output);
    
  private:
    CTcpClientSocket m_socket;
    std::string      m_address;
    int              m_port;
    std::string      m_error;
    CMessageQueue    m_messagequeue;
    CClock           m_clock;
    int              m_usectimeout;

    bool             ReadDataToQueue();
    bool             WriteDataToSocket(std::string strdata);
    bool             ParseWord(CMessage& message, std::string wordtocmp);
    bool             ParseWord(CMessage& message, std::string wordtocmp, std::string readword);
    bool             ParseLights(CMessage& message);
    bool             CheckLightExists(int lightnr, bool printerror = true);

    std::vector<CLight> m_lights;

    std::vector<std::string> m_options;
    std::string              m_lastoption; //place to store the last option retrieved by GetOption
};

#endif //CBOBLIGHT