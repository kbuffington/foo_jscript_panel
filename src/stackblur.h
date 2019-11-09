#pragma once

/*
The Stack Blur Algorithm was invented by Mario Klingemann,
mario@quasimondo.com and described here:
http://incubator.quasimondo.com/processing/fast_blur_deluxe.php

This is C++ RGBA (32 bit color) multi-threaded version
by Victor Laskin (victor.laskin@gmail.com)
More details: http://vitiy.info/stackblur-algorithm-multi-threaded-blur-for-cpp
*/

static unsigned short const stackblur_mul[255] =
{
	512,512,456,512,328,456,335,512,405,328,271,456,388,335,292,512,
	454,405,364,328,298,271,496,456,420,388,360,335,312,292,273,512,
	482,454,428,405,383,364,345,328,312,298,284,271,259,496,475,456,
	437,420,404,388,374,360,347,335,323,312,302,292,282,273,265,512,
	497,482,468,454,441,428,417,405,394,383,373,364,354,345,337,328,
	320,312,305,298,291,284,278,271,265,259,507,496,485,475,465,456,
	446,437,428,420,412,404,396,388,381,374,367,360,354,347,341,335,
	329,323,318,312,307,302,297,292,287,282,278,273,269,265,261,512,
	505,497,489,482,475,468,461,454,447,441,435,428,422,417,411,405,
	399,394,389,383,378,373,368,364,359,354,350,345,341,337,332,328,
	324,320,316,312,309,305,301,298,294,291,287,284,281,278,274,271,
	268,265,262,259,257,507,501,496,491,485,480,475,470,465,460,456,
	451,446,442,437,433,428,424,420,416,412,408,404,400,396,392,388,
	385,381,377,374,370,367,363,360,357,354,350,347,344,341,338,335,
	332,329,326,323,320,318,315,312,310,307,304,302,299,297,294,292,
	289,287,285,282,280,278,275,273,271,269,267,265,263,261,259
};

static BYTE const stackblur_shr[255] =
{
	9, 11, 12, 13, 13, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17,
	17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19,
	19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21,
	21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
	21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22,
	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23,
	23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
	23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
	23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
	23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24
};

/// Stackblur algorithm body
void stackblurJob(BYTE* src, ///< input image data
	t_size w, ///< image width
	t_size h, ///< image height
	t_size radius, ///< blur intensity (should be in 2..254 range)
	t_size cores, ///< total number of working threads
	t_size core, ///< current thread number
	t_size step, ///< step of processing (1,2)
	BYTE* stack ///< stack buffer
)
{
	t_size x, y, xp, yp, i;
	t_size sp;
	t_size stack_start;
	BYTE* stack_ptr;

	BYTE* src_ptr;
	BYTE* dst_ptr;

	unsigned long sum_r;
	unsigned long sum_g;
	unsigned long sum_b;
	unsigned long sum_a;
	unsigned long sum_in_r;
	unsigned long sum_in_g;
	unsigned long sum_in_b;
	unsigned long sum_in_a;
	unsigned long sum_out_r;
	unsigned long sum_out_g;
	unsigned long sum_out_b;
	unsigned long sum_out_a;

	t_size wm = w - 1;
	t_size hm = h - 1;
	t_size w4 = w * 4;
	t_size div = (radius * 2) + 1;
	t_size mul_sum = stackblur_mul[radius];
	BYTE shr_sum = stackblur_shr[radius];

	if (step == 1)
	{
		t_size minY = core * h / cores;
		t_size maxY = (core + 1) * h / cores;

		for (y = minY; y < maxY; y++)
		{
			sum_r = sum_g = sum_b = sum_a =
				sum_in_r = sum_in_g = sum_in_b = sum_in_a =
				sum_out_r = sum_out_g = sum_out_b = sum_out_a = 0;

			src_ptr = src + w4 * y; // start of line (0,y)

			for (i = 0; i <= radius; ++i)
			{
				stack_ptr = &stack[4 * i];
				stack_ptr[0] = src_ptr[0];
				stack_ptr[1] = src_ptr[1];
				stack_ptr[2] = src_ptr[2];
				stack_ptr[3] = src_ptr[3];
				sum_r += src_ptr[0] * (i + 1);
				sum_g += src_ptr[1] * (i + 1);
				sum_b += src_ptr[2] * (i + 1);
				sum_a += src_ptr[3] * (i + 1);
				sum_out_r += src_ptr[0];
				sum_out_g += src_ptr[1];
				sum_out_b += src_ptr[2];
				sum_out_a += src_ptr[3];
			}

			for (i = 1; i <= radius; ++i)
			{
				if (i <= wm) src_ptr += 4;
				stack_ptr = &stack[4 * (i + radius)];
				stack_ptr[0] = src_ptr[0];
				stack_ptr[1] = src_ptr[1];
				stack_ptr[2] = src_ptr[2];
				stack_ptr[3] = src_ptr[3];
				sum_r += src_ptr[0] * (radius + 1 - i);
				sum_g += src_ptr[1] * (radius + 1 - i);
				sum_b += src_ptr[2] * (radius + 1 - i);
				sum_a += src_ptr[3] * (radius + 1 - i);
				sum_in_r += src_ptr[0];
				sum_in_g += src_ptr[1];
				sum_in_b += src_ptr[2];
				sum_in_a += src_ptr[3];
			}

			sp = radius;
			xp = radius;
			if (xp > wm) xp = wm;
			src_ptr = src + 4 * (xp + y * w); // img.pix_ptr(xp, y);
			dst_ptr = src + y * w4; // img.pix_ptr(0, y);
			for (x = 0; x < w; x++)
			{
				dst_ptr[0] = static_cast<BYTE>((sum_r * mul_sum) >> shr_sum);
				dst_ptr[1] = static_cast<BYTE>((sum_g * mul_sum) >> shr_sum);
				dst_ptr[2] = static_cast<BYTE>((sum_b * mul_sum) >> shr_sum);
				dst_ptr[3] = static_cast<BYTE>((sum_a * mul_sum) >> shr_sum);
				dst_ptr += 4;

				sum_r -= sum_out_r;
				sum_g -= sum_out_g;
				sum_b -= sum_out_b;
				sum_a -= sum_out_a;

				stack_start = sp + div - radius;
				if (stack_start >= div) stack_start -= div;
				stack_ptr = &stack[4 * stack_start];

				sum_out_r -= stack_ptr[0];
				sum_out_g -= stack_ptr[1];
				sum_out_b -= stack_ptr[2];
				sum_out_a -= stack_ptr[3];

				if (xp < wm)
				{
					src_ptr += 4;
					++xp;
				}

				stack_ptr[0] = src_ptr[0];
				stack_ptr[1] = src_ptr[1];
				stack_ptr[2] = src_ptr[2];
				stack_ptr[3] = src_ptr[3];

				sum_in_r += src_ptr[0];
				sum_in_g += src_ptr[1];
				sum_in_b += src_ptr[2];
				sum_in_a += src_ptr[3];
				sum_r += sum_in_r;
				sum_g += sum_in_g;
				sum_b += sum_in_b;
				sum_a += sum_in_a;

				++sp;
				if (sp >= div) sp = 0;
				stack_ptr = &stack[sp * 4];

				sum_out_r += stack_ptr[0];
				sum_out_g += stack_ptr[1];
				sum_out_b += stack_ptr[2];
				sum_out_a += stack_ptr[3];
				sum_in_r -= stack_ptr[0];
				sum_in_g -= stack_ptr[1];
				sum_in_b -= stack_ptr[2];
				sum_in_a -= stack_ptr[3];
			}
		}
	}

	// step 2
	if (step == 2)
	{
		t_size minX = core * w / cores;
		t_size maxX = (core + 1) * w / cores;

		for (x = minX; x < maxX; x++)
		{
			sum_r = sum_g = sum_b = sum_a =
				sum_in_r = sum_in_g = sum_in_b = sum_in_a =
				sum_out_r = sum_out_g = sum_out_b = sum_out_a = 0;

			src_ptr = src + 4 * x; // x,0
			for (i = 0; i <= radius; ++i)
			{
				stack_ptr = &stack[i * 4];
				stack_ptr[0] = src_ptr[0];
				stack_ptr[1] = src_ptr[1];
				stack_ptr[2] = src_ptr[2];
				stack_ptr[3] = src_ptr[3];
				sum_r += src_ptr[0] * (i + 1);
				sum_g += src_ptr[1] * (i + 1);
				sum_b += src_ptr[2] * (i + 1);
				sum_a += src_ptr[3] * (i + 1);
				sum_out_r += src_ptr[0];
				sum_out_g += src_ptr[1];
				sum_out_b += src_ptr[2];
				sum_out_a += src_ptr[3];
			}
			for (i = 1; i <= radius; ++i)
			{
				if (i <= hm) src_ptr += w4; // +stride

				stack_ptr = &stack[4 * (i + radius)];
				stack_ptr[0] = src_ptr[0];
				stack_ptr[1] = src_ptr[1];
				stack_ptr[2] = src_ptr[2];
				stack_ptr[3] = src_ptr[3];
				sum_r += src_ptr[0] * (radius + 1 - i);
				sum_g += src_ptr[1] * (radius + 1 - i);
				sum_b += src_ptr[2] * (radius + 1 - i);
				sum_a += src_ptr[3] * (radius + 1 - i);
				sum_in_r += src_ptr[0];
				sum_in_g += src_ptr[1];
				sum_in_b += src_ptr[2];
				sum_in_a += src_ptr[3];
			}

			sp = radius;
			yp = radius;
			if (yp > hm) yp = hm;
			src_ptr = src + 4 * (x + yp * w); // img.pix_ptr(x, yp);
			dst_ptr = src + 4 * x; // img.pix_ptr(x, 0);
			for (y = 0; y < h; y++)
			{
				dst_ptr[0] = static_cast<BYTE>((sum_r * mul_sum) >> shr_sum);
				dst_ptr[1] = static_cast<BYTE>((sum_g * mul_sum) >> shr_sum);
				dst_ptr[2] = static_cast<BYTE>((sum_b * mul_sum) >> shr_sum);
				dst_ptr[3] = static_cast<BYTE>((sum_a * mul_sum) >> shr_sum);
				dst_ptr += w4;

				sum_r -= sum_out_r;
				sum_g -= sum_out_g;
				sum_b -= sum_out_b;
				sum_a -= sum_out_a;

				stack_start = sp + div - radius;
				if (stack_start >= div) stack_start -= div;
				stack_ptr = &stack[4 * stack_start];

				sum_out_r -= stack_ptr[0];
				sum_out_g -= stack_ptr[1];
				sum_out_b -= stack_ptr[2];
				sum_out_a -= stack_ptr[3];

				if (yp < hm)
				{
					src_ptr += w4; // stride
					++yp;
				}

				stack_ptr[0] = src_ptr[0];
				stack_ptr[1] = src_ptr[1];
				stack_ptr[2] = src_ptr[2];
				stack_ptr[3] = src_ptr[3];

				sum_in_r += src_ptr[0];
				sum_in_g += src_ptr[1];
				sum_in_b += src_ptr[2];
				sum_in_a += src_ptr[3];
				sum_r += sum_in_r;
				sum_g += sum_in_g;
				sum_b += sum_in_b;
				sum_a += sum_in_a;

				++sp;
				if (sp >= div) sp = 0;
				stack_ptr = &stack[sp * 4];

				sum_out_r += stack_ptr[0];
				sum_out_g += stack_ptr[1];
				sum_out_b += stack_ptr[2];
				sum_out_a += stack_ptr[3];
				sum_in_r -= stack_ptr[0];
				sum_in_g -= stack_ptr[1];
				sum_in_b -= stack_ptr[2];
				sum_in_a -= stack_ptr[3];
			}
		}
	}
}

class stack_blur_task : public pfc::thread
{
public:
	BYTE* src;
	t_size w;
	t_size h;
	t_size radius;
	t_size cores;
	t_size core;
	t_size step;
	BYTE* stack;

	inline stack_blur_task(BYTE* src, t_size w, t_size h, t_size radius, t_size cores, t_size core, t_size step, BYTE* stack)
	{
		this->src = src;
		this->w = w;
		this->h = h;
		this->radius = radius;
		this->cores = cores;
		this->core = core;
		this->step = step;
		this->stack = stack;
	}

	inline void threadProc() override
	{
		stackblurJob(src, w, h, radius, cores, core, step, stack);
	}
};

void stackblur(BYTE* src, ///< input image data
	t_size w, ///< image width
	t_size h, ///< image height
	t_size radius ///< blur intensity (should be in 2..254 range)
)
{
	t_size cores = std::max(1, (int)pfc::getOptimalWorkerThreadCount());
	t_size div = (radius * 2) + 1;
	BYTE* stack = new BYTE[div * 4 * cores];

	if (cores == 1)
	{
		// no multithreading
		stackblurJob(src, w, h, radius, 1, 0, 1, stack);
		stackblurJob(src, w, h, radius, 1, 0, 2, stack);
	}
	else
	{
		t_size i;
		stack_blur_task** workers = new stack_blur_task*[cores];
		for (i = 0; i < cores; ++i)
		{
			workers[i] = new stack_blur_task(src, w, h, radius, cores, i, 1, stack + div * 4 * i);
			workers[i]->start();
		}

		for (i = 0; i < cores; ++i)
		{
			workers[i]->waitTillDone();
		}

		for (i = 0; i < cores; ++i)
		{
			workers[i]->step = 2;
			workers[i]->start();
		}

		for (i = 0; i < cores; ++i)
		{
			workers[i]->waitTillDone();
			delete workers[i];
		}

		delete[] workers;
	}

	delete[] stack;
}

void stack_blur_filter(Gdiplus::Bitmap& img, BYTE radius) throw()
{
	if (radius < 2) radius = 2;
	if (radius > 254) radius = 254;
	t_size width = img.GetWidth();
	t_size height = img.GetHeight();

	Gdiplus::Rect rect(0, 0, width, height);
	Gdiplus::BitmapData bmpdata;

	if (img.LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppPARGB, &bmpdata) == Gdiplus::Ok)
	{
		stackblur(static_cast<BYTE*>(bmpdata.Scan0), width, height, radius);
		img.UnlockBits(&bmpdata);
	}
}
