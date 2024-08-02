module;

#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>

export module camera;

export class Camera {
	bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
	bx::Vec3 eye = { 0.0f, 0.0f, 0.0f };

	int width;
	int height;

public:
	Camera()
	{
		width = 0;
		height = 0;
	}


	void init() 
	{
		at = { 0.0f, 0.0f, 0.0f };
		eye = { 0.0f, 0.0f, 10.0f };
	}

	void set_window_size(int w, int h)
	{
		width = w;
		height = h;
	}


	void apply() const
	{
		// Set view and projection matrix for view 0.
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj,
			60.0f,
			float(width) / float(height),
			0.1f, 100.0f,
			bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewTransform(0, view, proj);
	}
};