#include "shop_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Network/NetworkDriver.h>
#include <Core/Core.h>
#include <Utilities/Input.h>
#include <cmath>
#include <psppower.h>
#include <zip.h>

using namespace Stardust;

std::vector<std::string> extracted_vector;

void shop_state::init()
{
    m_should_change = false;

    m_box_width = 110;
    m_box_height = 67;

    m_selection_index = 0;
    m_selection_index_s = 0;

    m_page = 0;
    m_description = "";

    json_vector.clear();
    extracted_vector.clear();

    box_sprite = new GFX::Render2D::Sprite(g::textures::bar);
    bar_box_sprite = new GFX::Render2D::Sprite(g::textures::bar);

    box_sprite->setColor(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f);
    box_sprite->setScale((float)m_box_width / 16.0f, (float)m_box_height / 16.0f);

    bar_box_sprite->setScale(100, 2.2f);

    bar_box_sprite->setColor(64 / 255.f, 123 / 255.f, 181 / 255.f, 1.f);
    box_sprite->setColor(66 / 255.f, 66 / 255.f, 66 / 255.f, 1.f);
    //bar_box_sprite->setPosition((5 * 100) / 2, (5 * 2) / 4);
}

void shop_state::destroy()
{
}

// PROUDLY
// stolen from iridescent corpration
std::string text_wrap(std::string str, int location)
{
    // your other code
    int n = str.rfind(' ', location);
    if (n != std::string::npos)
    {
        str.at(n) = '\n';
    }
    // your other code
    return str;
}

void shop_state::load_showcase_data() {
    Json::Value v = Utilities::JSON::openJSON("cache/apps.json");
    m_description = v["apps"][m_selection_index]["desc"].asString();

    int min_wraps = m_description.size() / 40;
    for (int i = 0; i < min_wraps; i++)
    {
        m_description = text_wrap(m_description, 40 * (i + 1));
    }
}

void shop_state::update()
{
    // input shit
    switch (m_page)
    {
    case 0:
        if (Utilities::KeyPressed(PSP_CTRL_UP))
        {
            m_selection_index -= 4;
            if (m_selection_index < 0)
                m_selection_index = 0;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_DOWN))
        {
            m_selection_index += 4;
            if (m_selection_index > json_vector.size() - 1)
                m_selection_index = json_vector.size() - 1;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_RIGHT))
        {
            m_selection_index += 1;
            if (m_selection_index > json_vector.size() - 1)
                m_selection_index = json_vector.size() - 1;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_LEFT))
        {
            m_selection_index -= 1;
            if (m_selection_index < 0)
                m_selection_index = 0;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_CROSS))
        {
            m_page += 1;
            if (m_page > 2)
                m_page = 2;

            load_showcase_data();
        }
        else if (Utilities::KeyPressed(PSP_CTRL_CIRCLE))
        {
            m_page -= 1;
            if (m_page < 0)
                m_page = 0;
        }
        break;
    case 1:
        if (Utilities::KeyPressed(PSP_CTRL_RIGHT))
        {
            m_selection_index_s += 1;
            if (m_selection_index_s > 1)
                m_selection_index_s = 1;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_LEFT))
        {
            m_selection_index_s -= 1;
            if (m_selection_index_s < 0)
                m_selection_index_s = 0;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_CROSS))
        {
            m_page += 1;
            
            if (m_selection_index_s == 1)
                m_uninstall = true;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_CIRCLE))
        {
            m_page -= 1;
            if (m_page < 0)
                m_page = 0;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_RTRIGGER))
        {
            m_selection_index += 1;
            if (m_selection_index > json_vector.size() - 1)
                m_selection_index = json_vector.size() - 1;
            
            load_showcase_data();
        }
        else if (Utilities::KeyPressed(PSP_CTRL_LTRIGGER))
        {
            m_selection_index -= 1;
            if (m_selection_index < 0)
                m_selection_index = 0;
            
            load_showcase_data();
        }
    case 2:
        if (Utilities::KeyPressed(PSP_CTRL_CROSS))
        {
        }
        else if (Utilities::KeyPressed(PSP_CTRL_CIRCLE))
        {
            m_page -= 1;
        }
    default:
        break;
    }

    if (m_page > 2)
        m_page = 2;
    
    if (m_page < 0)
        m_page = 0;

    static bool once = true;
    if (once)
    {
        once = false;

        Json::Value v = Utilities::JSON::openJSON("cache/apps.json");
        for (int i = 0; i < v["apps"].size(); i++)
        {
            if (v["apps"][i].isObject())
            {
                std::string big_thumb = "cache/" + v["apps"][i]["short"].asString() + "/icon.png";
                unsigned int sextillion = GFX::g_TextureManager->loadTex(big_thumb, GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, false);
                GFX::Render2D::Sprite *temp_sprite = new GFX::Render2D::Sprite(sextillion, {0, 0}, {480, 272});
                temp_sprite->setScale((float)m_box_width / 480.0f, (float)m_box_height / 272.0f);

                app_data data;
                data.short_name = v["apps"][i]["short"].asString();
                data.long_name = v["apps"][i]["name"].asString();
                data.sprite = temp_sprite;

                json_vector.push_back(data);
            }
        }
    }
}

/*
    // how to download
    download to /temp/appname.zip
    extract to /PSP/GAME/appname/ *stuff*

    appname_v1.0.zip -> psp -> extract ms0:/PSP/GAME/appname
    + zip/compression library for the psp

    // how to upload a app
    send 
        appname
        github link
        description
        extra info
        1 big thumbnail
        max 4 screen shots
        app zipped every single thing included, zip file should be named like so (appname.zip)
    through email

    1. grey out the download button if downloaded
    2. charging, battery icon
    3. category lite support

    - zipping/unzipping
    - downloading .zip files
    - extracting to /PSP/GAME/appname

*/

bool shop_state::should_change()
{
    return m_should_change;
}

int on_extract_entry(const char *filename, void *arg)
{
    static int i = 0;
    int n = *(int *)arg;
    
    std::stringstream stream;
    stream << "Extracted: " << filename << " (" << ++i << " of " << n << ")";

    extracted_vector.push_back(stream.str());

    return 0;
}

bool file_exists(std::string filename) {
    return std::ifstream(filename).good();
}

void shop_state::draw()
{
    static bool once_but_again = true;
    
    static float progress_bar_width = 1.f;
    static int m_loading_text_alpha = 0;
    static std::string m_what_is_going_on = "loading";

    if (m_page == 0)
    {
        // x, y for the selection box
        int y = m_selection_index / 4;
        int x = m_selection_index - y * 4;

        // for maxing alpha of the selection box when switching
        static int prev_x = x;
        static int prev_y = y;
        static int select_box_alpha = 255;

        // flashing
        if (select_box_alpha > 0.f)
        {
            select_box_alpha -= 2;
        }
        else if (select_box_alpha <= 0.f)
        {
            select_box_alpha = 255;
        }

        // max alpha when switching
        if (prev_x != x || prev_y != y)
        {
            select_box_alpha = 255;
            prev_x = x;
            prev_y = y;
        }
        else
        {
            prev_x = x;
            prev_y = y;
        }

        // set the color of the box white, change alpha, scale it
        box_sprite->setColor(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, select_box_alpha / 255.f);
        box_sprite->setScale((float)(m_box_width + 6) / 16.0f, (float)(m_box_height + 6) / 16.0f);

        // calculations for the selection box
        float x_padding = (x + 1) * 8.5f;
        float x_box_width_pos = x * m_box_width;
        float x_center_to_left_align = m_box_width / 2;

        float y_padding = (y + 1) * 6.5f;
        float y_name_padding = y * 8.f;
        float y_box_width_pos = y * m_box_height;
        float y_center_to_left_align = m_box_height / 2;

        // set the selection box position to be on the selection index
        box_sprite->setPosition(x_box_width_pos + x_center_to_left_align + x_padding, 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding));

        box_sprite->draw();

        // make fonts 0.5f size
        g::font_renderer->setStyle({255, 255, 255, 255, 0.5f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});

        int y_size = json_vector.size() / 4;
        for (int y = 0; y <= y_size; y++)
        {
            int x_size = (y < y_size) ? 4 : json_vector.size() - y * 4;
            for (int x = 0; x < x_size; x++)
            {
                float x_padding = (x + 1) * 8.5f;
                float x_box_width_pos = x * m_box_width;
                float x_center_to_left_align = m_box_width / 2;

                float y_padding = (y + 1) * 6.5f;
                float y_name_padding = y * 8.f;
                float y_box_width_pos = y * m_box_height;
                float y_center_to_left_align = m_box_height / 2;

                json_vector.at(y * 4 + x).sprite->setScale((float)m_box_width / 480.0f, (float)m_box_height / 272.0f);
                json_vector.at(y * 4 + x).sprite->setPosition(x_box_width_pos + x_center_to_left_align + x_padding, 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding));
                json_vector.at(y * 4 + x).sprite->draw();

                g::font_renderer->draw(json_vector.at(y * 4 + x).long_name, {x_box_width_pos + x_center_to_left_align + x_padding, 40 + 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding)});
            }
        }

        // return style back to normal
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});
    }
    else if (m_page == 1)
    {
        float x_padding = 1 * 8.5f;
        float x_box_width_pos = 0 * m_box_width * 2;
        float x_center_to_left_align = m_box_width;

        float y_padding = 1 * 6.5f;
        float y_name_padding = 0 * 8.f;
        float y_box_width_pos = 0 * m_box_height * 2;
        float y_center_to_left_align = m_box_height;

        /* app thumbnail */
        json_vector.at(m_selection_index).sprite->setScale((float)m_box_width / 240.0f, (float)m_box_height / 136.0f);
        json_vector.at(m_selection_index).sprite->setPosition(x_box_width_pos + x_center_to_left_align + x_padding, 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding));
        json_vector.at(m_selection_index).sprite->draw();

        /* app name */
        g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, TEXT_RENDERER_LEFT, TEXT_RENDERER_LEFT, 0, false});
        g::font_renderer->draw(json_vector.at(m_selection_index).long_name, {x_box_width_pos * 2 + x_center_to_left_align * 2 + x_padding * 2, 10 + 18 + y_name_padding + (y_box_width_pos + y_padding)});

        /* divider */
        box_sprite->setColor(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.f);
        box_sprite->setScale((float)(m_box_width * 2 + 15) / 16.0f, (float)(m_box_height - 66.5f) / 16.0f);
        box_sprite->setPosition(x_box_width_pos * 4 + x_center_to_left_align * 3 + x_padding * 3 - 3, 18 + 18 + y_name_padding + (y_box_width_pos + y_padding));
        box_sprite->draw();

        /* description */
        g::font_renderer->setStyle({255, 255, 255, 255, 0.4f, TEXT_RENDERER_LEFT, TEXT_RENDERER_LEFT, 0, false});
        g::font_renderer->draw(m_description, {x_box_width_pos * 2 + x_center_to_left_align * 2 + x_padding * 2, 25 + 18 + y_name_padding + (y_box_width_pos + y_padding)});
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});

        x_padding = 1 * 8.5f;
        x_center_to_left_align = 64;

        y_padding = 1 * 6.5f;
        y_center_to_left_align = 32;

        static int select_box_alpha = 255;

        // flashing
        if (select_box_alpha > 0.f)
        {
            select_box_alpha -= 2;
        }
        else if (select_box_alpha <= 0.f)
        {
            select_box_alpha = 255;
        }

        // x, y for the selection box
        int x = m_selection_index_s;

        // calculations for the selection box
        x_padding = (x + 1) * 8.5f;
        x_box_width_pos = x * m_box_width;
        if (x == 1)
            x_box_width_pos += 25;

        /* selection flashing thingy */
        // set the color of the box white, change alpha, scale it
        box_sprite->setColor(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, select_box_alpha / 255.f);
        box_sprite->setScale((float)(128 + 6) / 16.0f, (float)(64 + 6) / 16.0f);

        // set the selection box position to be on the selection index
        box_sprite->setPosition(x_center_to_left_align + x_box_width_pos + x_padding + 1, m_box_height * 2 + 18 + y_center_to_left_align + y_padding * 2);

        box_sprite->draw();

        // dadada
        x_padding = 1 * 8.5f;
        y_padding = 1 * 6.5f;

        /* download button */
        //image_sprite->setPosition(x_center_to_left_align + x_padding + 1, m_box_height * 2 + 18 + y_center_to_left_align + y_padding * 2);
        //image_sprite->draw();

        if (x == 0)
            g::font_renderer->setStyle({255 - select_box_alpha, 255 - select_box_alpha, 255 - select_box_alpha, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});
        g::font_renderer->draw("download", {x_center_to_left_align + x_padding + 1, m_box_height * 2 + 18 + y_center_to_left_align + y_padding * 2});
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});

        /* uninstall button */
        if (x == 1)
            g::font_renderer->setStyle({255 - select_box_alpha, 255 - select_box_alpha, 255 - select_box_alpha, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});
        g::font_renderer->draw("uninstall", {x_center_to_left_align + x_padding + 1 + m_box_width + 36, m_box_height * 2 + 18 + y_center_to_left_align + y_padding * 2});
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});

        // set the color of the box white, change alpha, scale it
        box_sprite->setColor(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, select_box_alpha / 255.f);
        box_sprite->setScale((float)(m_box_width + 6) / 16.0f, (float)(m_box_height + 6) / 16.0f);
        box_sprite->setColor(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.f);

        once_but_again = true;
        m_loading_text_alpha = 255;
        progress_bar_width = 0;
        m_what_is_going_on = "";
    }
    else if (m_page == 2)
    {
        if (!m_uninstall)
        {
            if (m_loading_text_alpha > 0.f)
            {
                m_loading_text_alpha -= 2;
            }
            else if (m_loading_text_alpha <= 0.f)
            {
                m_loading_text_alpha = 255;
            }

            g::font_renderer->setStyle({ 255, 255, 255, m_loading_text_alpha, 1.f, -1, TEXT_RENDERER_CENTER, 0.0f, true });
            g::font_renderer->draw(m_what_is_going_on, { 240, 106 });

            box_sprite->setPosition(240, 136);
            box_sprite->setColor(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.f);
            box_sprite->setScale(progress_bar_width, 1.f);
            box_sprite->draw();

            g::font_renderer->setStyle({ 255, 255, 255, 255, 0.5f, -1, TEXT_RENDERER_CENTER, 0.0f, true });
            int line_counter = 0;
            for (auto &&i : extracted_vector)
            {
                g::font_renderer->draw(i, { 240, 180 + (line_counter * 10) });
                line_counter++;
            }

            if (once_but_again)
            {
                m_what_is_going_on = "loading";
                once_but_again = false;
                progress_bar_width += 5.f;
                std::string base_url = "http://psphbshop.s3.eu-central-1.amazonaws.com/";
                std::string file_to_get_url = base_url + "cache/" + json_vector.at(m_selection_index).short_name + "/app.zip";
                std::string file_to_get = "cache/" + json_vector.at(m_selection_index).short_name + "/app.zip";

                Utilities::app_Logger->log(file_to_get_url);
                #ifndef SKIP_NET_INIT
                Network::g_NetworkDriver.GetFileHTTP(file_to_get_url.c_str(), file_to_get.c_str());
                #endif

                if (!file_exists(file_to_get.c_str())) {
                    progress_bar_width = 0;
                    m_what_is_going_on = "file to extract doesn't exists, download error";
                }

                progress_bar_width += 5.f;
                struct zip_t *zip = zip_open(file_to_get.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
                {
                    int arg = 2;
                    zip_extract(file_to_get.c_str(), "../", on_extract_entry, &arg);
                }
                progress_bar_width = 29.f;
                m_what_is_going_on = "done extracting";
            }

            box_sprite->setScale((float)(m_box_width + 6) / 16.0f, (float)(m_box_height + 6) / 16.0f);
        }
        else {
            m_uninstall = false;
        }
    }
    // status bar
    bar_box_sprite->setPosition(0, 0);
    bar_box_sprite->draw();

    // battery life calculation
    int life = scePowerGetBatteryLifeTime();
    int h = (life / 100) + 2;
    int m = (life / 10);
    std::string battery_life = "";
    if ((life / 100) != 0)
        battery_life = std::to_string(h) + "h" + std::to_string(m) + "m";
    else if ((life / 100) != 0)
        battery_life = "full";

    // get clock
    char time_text[25];
    pspTime time;
    sceRtcGetCurrentClockLocalTime(&time);

    // set clock text
    int hour = time.hour % 12;
    int am_pm = time.hour / 12;
    snprintf(time_text, 25, "%2i:%02i %s", (hour == 0) ? 12 : hour, time.minutes, am_pm ? "pm" : "am");

    // render time
    g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, TEXT_RENDERER_RIGHT, TEXT_RENDERER_LEFT, 0, false});
    //g::font_renderer->draw(time_text, {5, 10});

    // render charging indicator
    g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, TEXT_RENDERER_RIGHT, TEXT_RENDERER_RIGHT, 0, false});
    g::font_renderer->draw((scePowerIsBatteryCharging() ? "charging " : "") + battery_life + " " + std::to_string(scePowerGetBatteryLifePercent()) + "%", {475, 10});

    g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});

    bar_box_sprite->setPosition(0, 272);
    bar_box_sprite->draw();

    g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, TEXT_RENDERER_LEFT, TEXT_RENDERER_LEFT, 0, false});
    g::font_renderer->draw("X: select, O: back, DPAD: move", {5, 265});

    g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});
}