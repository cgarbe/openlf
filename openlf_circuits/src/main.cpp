/*
* Copyright (c) 2015 Heidelberg Colaboratory for Image Processing
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Author Maximilian Diebold
*/

#include <iostream>
#include <QApplication>
#include <QTextEdit>
#include "circuit_demo.h"
#include <QtWidgets>


int main(int argv, char **args)
{
	
	QApplication app(argv, args);
	app.setOrganizationName("HCI");
	app.setApplicationName("Demo Program");

	QWidget window;
	window.resize(320, 240);
	window.show();
	window.setWindowTitle(QApplication::translate("toplevel", "Top-level widget"));
	//QtpMain tool;
	circuit_demo demo;
	//demo.show(tool);


	demo.run();

	DSPatch::Finalize();

	return app.exec();
}