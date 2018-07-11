#pragma once

#include "../Window.hpp"

namespace Goo
{
class GLWindow : public Window
{
public:
  void swapBuffers();

	GLContext& context() { return _context; }
	const GLContext& context() const { return _context; }

  void setViewport(const Point& pos, const Size& size);

  void setFullscreen(bool enable);
  void setVsync(bool enable);

private:
  void createControl() override;

  GLContext _context;
};
}
