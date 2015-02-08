/*************************************************************************/
/*  main_loop.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "main_loop.h"
#include "script_language.h"

void MainLoop::_bind_methods() {

	ObjectTypeDB::bind_method("input_event",&MainLoop::input_event);

	BIND_CONSTANT(NOTIFICATION_WM_FOCUS_IN);
	BIND_CONSTANT(NOTIFICATION_WM_FOCUS_OUT);
	BIND_CONSTANT(NOTIFICATION_WM_QUIT_REQUEST);
	BIND_CONSTANT(NOTIFICATION_WM_UNFOCUS_REQUEST);
	BIND_CONSTANT(NOTIFICATION_OS_MEMORY_WARNING);

};

void MainLoop::set_init_script(const Ref<Script>& p_init_script) {

	init_script=p_init_script;
}

MainLoop::MainLoop() {
}


MainLoop::~MainLoop()
{
}



void MainLoop::input_text( const String& p_text ) {


}

void MainLoop::input_event( const InputEvent& p_event ) {

	if (get_script_instance())
		get_script_instance()->call("input_event",p_event);

}

void MainLoop::init() {

	if (init_script.is_valid())
		set_script(init_script.get_ref_ptr());

	if (get_script_instance())
		get_script_instance()->call("init");

}
bool MainLoop::iteration(float p_time) {

	if (get_script_instance())
		return get_script_instance()->call("iteration",p_time);

	return false;

}
bool MainLoop::idle(float p_time) {

	if (get_script_instance())
		return get_script_instance()->call("idle",p_time);

	return false;
}
void MainLoop::finish() {

	if (get_script_instance()) {
		get_script_instance()->call("finish");
		set_script(RefPtr()); //clear script
	}


}

