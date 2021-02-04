/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2020  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "JoystickConfiguration.h"

// Constructor of JoystickConfiguration
JoystickConfiguration::JoystickConfiguration(int pad, bool left, wxWindow* parent)
	: wxDialog(parent, wxID_ANY, _T("Joystick configuration"), wxDefaultPosition, wxDefaultSize,
	           wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	m_pad_id = pad;
	m_isForLeftJoystick = left;

	wxBoxSizer* joy_conf_box = new wxBoxSizer(wxVERTICAL);

	if (m_isForLeftJoystick)
	{
		m_cb_reverse_x = new wxCheckBox(this, wxID_ANY, _T("Reverse Lx"));
		m_cb_reverse_y = new wxCheckBox(this, wxID_ANY, _T("Reverse Ly"));
		m_cb_mouse_joy = new wxCheckBox(this, wxID_ANY, _T("Use mouse for left analog joystick"));
	}
	else
	{
		m_cb_reverse_x = new wxCheckBox(this, wxID_ANY, _T("Reverse Rx"));
		m_cb_reverse_y = new wxCheckBox(this, wxID_ANY, _T("Reverse Ry"));
		m_cb_mouse_joy = new wxCheckBox(this, wxID_ANY, _T("Use mouse for right analog joystick"));
	}

	joy_conf_box->Add(m_cb_reverse_x, wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT, 5));
	joy_conf_box->Add(m_cb_reverse_y, wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT, 5));
	joy_conf_box->Add(m_cb_mouse_joy, wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT, 5));

	joy_conf_box->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), wxSizerFlags().Border(wxALL, 5).Right());

	SetSizerAndFit(joy_conf_box);
}

/**
 * Initialize the frame
 * Check if a gamepad is detected
 */
void JoystickConfiguration::InitJoystickConfiguration()
{
	repopulate(); // Set label and fit simulated key array
	/*
	 * Check if there exist at least one pad available
	 * if the pad id is 0, you need at least 1 gamepad connected,
	 * if the pad id is 1, you need at least 2 gamepads connected,
	 * Prevent to use a none initialized value on s_vgamePad (core dump)
	 */
	if (s_vgamePad.size() < m_pad_id + 1)
	{
		if (s_vgamePad.empty())
			wxMessageBox(L"No gamepad detected.");
		else
			wxMessageBox(L"No second gamepad detected.");

		// disable all checkboxes
		m_cb_reverse_y->Disable();
		m_cb_reverse_x->Disable();
	}
}

/****************************************/
/*********** Methods functions **********/
/****************************************/

// Set button values
void JoystickConfiguration::repopulate()
{
	if (m_isForLeftJoystick)
	{
		m_cb_reverse_x->SetValue(g_conf.pad_options[m_pad_id].reverse_lx);
		m_cb_reverse_y->SetValue(g_conf.pad_options[m_pad_id].reverse_ly);
		m_cb_mouse_joy->SetValue(g_conf.pad_options[m_pad_id].mouse_l);
	}
	else
	{
		m_cb_reverse_x->SetValue(g_conf.pad_options[m_pad_id].reverse_rx);
		m_cb_reverse_y->SetValue(g_conf.pad_options[m_pad_id].reverse_ry);
		m_cb_mouse_joy->SetValue(g_conf.pad_options[m_pad_id].mouse_r);
	}
}

void JoystickConfiguration::Run()
{
	InitJoystickConfiguration();
	if (ShowModal() == wxID_OK)
	{
		if (m_isForLeftJoystick)
		{
			g_conf.pad_options[m_pad_id].reverse_lx = m_cb_reverse_x->GetValue();
			g_conf.pad_options[m_pad_id].reverse_ly = m_cb_reverse_y->GetValue();
			g_conf.pad_options[m_pad_id].mouse_l = m_cb_mouse_joy->GetValue();
		}
		else
		{
			g_conf.pad_options[m_pad_id].reverse_rx = m_cb_reverse_x->GetValue();
			g_conf.pad_options[m_pad_id].reverse_ry = m_cb_reverse_y->GetValue();
			g_conf.pad_options[m_pad_id].mouse_r = m_cb_mouse_joy->GetValue();
		}
	}
}
