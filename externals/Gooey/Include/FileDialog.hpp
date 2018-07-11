#pragma once

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

#include "Dialog.hpp"

namespace Goo
{
class OpenFileDialog
{
public:
	OpenFileDialog() : _text(nullptr), _filter(nullptr) {}
	OpenFileDialog(std::string text, std::string filter) : _text(std::move(text)), _filter(std::move(filter)) {}

	DialogResult show();
	std::string filename() const;

	void setText(std::string text);
	std::string text() const { return _text; }
	void setFilter(std::string filter);
	std::string filter() const { return _filter; }

private:
	std::string _text;
	std::string _filter;
	char _buffer[MAX_PATH] = { 0 };
};
}
