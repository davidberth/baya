module;

#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>

export module camera;

export class Camera {

	float x = 0.0f;
	float y = 0.0f;

	float height = 5.0f;
	float heading = 0.0f;

	bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
	bx::Vec3 eye = { 0.0f, 0.0f, 0.0f };

	float view[16] = { 0 };
	float proj[16] = { 0 };

	int window_width;
	int window_height;

public:
	Camera()
	{
		window_width = 0;
		window_height = 0;
	}

	void set_position(float tx, float ty)
	{
		x = tx;
		y = ty;
	}

	void move(float dx, float dy)
	{
		x += dx;
		y += dy;
	}

	void set_height(float h)
	{
		height = h;
	}

	void set_heading(float h)
	{
		heading = h;
	}


	void init() 
	{
		x = 0.0f;
		y = 0.0f;
		height = 5.0f;
		heading = 0.0f;
	}

	void set_window_size(int w, int h)
	{
		window_width = w;
		window_height = h;
	}


	void apply()
	{
		// Set view and projection matrix for view 0.
		eye = { x, y, height };
		at = { x, y , 0.0f };
		bx::mtxLookAt(view, eye, at);
		
		bx::mtxProj(proj,
			60.0f,
			float(window_width) / float(window_height),
			0.1f, 100.0f,
			bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewTransform(0, view, proj);
	}
};