//----------------------------------------------------------------------------
// Image Processing Library
//
// MIT License
// Copyright (c) 2018 Martin A Slater (mslater@hellinc.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image.h"
#include "functions/common.h"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc_c.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	//----------------------------------------------------------------------------

	image::image(Format format, int width, int height) :
		m_CVMat(nullptr)
	{
		cv::Mat mat(height, width, CV_8UC3);
		set_mat(mat, format);
	}

	//----------------------------------------------------------------------------

	image::image(const image& img) :
		m_CVMat(nullptr)
	{
		set_mat(img.get_opencv()->clone(), img.m_Format);
	}

	//----------------------------------------------------------------------------

	image::image(const char *src) :
		m_CVMat(nullptr),
		m_SourcePath(src)
	{
		IMAGE_PROC_ASSERT(src);

		auto mat = cv::imread(src, 1);
		if (mat.dims == 0)
			throw image_read_error(src);
		set_mat(mat, mat.channels() == 1 ? Format::Grey : Format::RGB);
	}

	//----------------------------------------------------------------------------

	image::image(const std::string& src) :
		image(src.c_str())
	{}

	//----------------------------------------------------------------------------

	image::~image()
	{
		IMAGE_PROC_SAFE_DELETE(m_CVMat);
	}

	//----------------------------------------------------------------------------

	image* image::clone() const
	{
		auto *c = new image();
		c->m_CVMat = new cv::Mat(get_opencv()->clone());
		c->m_Format = m_Format;
		c->m_SourcePath = m_SourcePath;
		return c;
	}

	//----------------------------------------------------------------------------

	cv::Mat* image::get_opencv()
	{
		return m_CVMat;
	}

	//----------------------------------------------------------------------------

	bool image::write_to_file(const char* path) const
	{
		return imwrite(path, *m_CVMat);
	}

	//----------------------------------------------------------------------------

	bool image::write_to_file(const std::string& path) const
	{
		return write_to_file(path.c_str());
	}

	//----------------------------------------------------------------------------

	int image::get_width() const
	{
		if (m_CVMat)
			return m_CVMat->size().width;
		return 0;
	}

	//----------------------------------------------------------------------------

	int image::get_height() const
	{
		if (m_CVMat)
			return m_CVMat->size().height;
		return 0;
	}

	//----------------------------------------------------------------------------

	image::Format image::get_format() const
	{
		return m_Format;
	}

	//----------------------------------------------------------------------------

	const std::string& image::get_source_path() const
	{
		return m_SourcePath;
	}

	//----------------------------------------------------------------------------

	void image::set_mat(const cv::Mat& mat, Format format)
	{
		IMAGE_PROC_SAFE_DELETE(m_CVMat);
		m_CVMat = new cv::Mat(mat);
		m_Format = format;
	}

	//----------------------------------------------------------------------------

	bool image::convert_to(image* dst, Format format)
	{
		// handle the cases where opencv does it directly
		const int Same = -2;
		const int Unimpl = -1;
		const int MultiToGrey = -3;
		const int GreyToMulti = -4;

		const int cv_fmts[static_cast<int>(Format::Count)][static_cast<int>(Format::Count)] = {
			/*             RGB	     LAB		  Grey	       HSV		    HLS */
			/* RGB  */{ Same,        CV_BGR2Lab,  CV_BGR2GRAY, CV_BGR2HSV,  CV_BGR2HLS },
			/* Lab  */{ CV_Lab2BGR,  Same,        MultiToGrey, Unimpl,      Unimpl },
			/* Grey */{ CV_GRAY2BGR, GreyToMulti, Same,        GreyToMulti, GreyToMulti },
			/* HSV  */{ CV_HSV2BGR,  Unimpl,      MultiToGrey, Same,        Unimpl },
			/* HLS  */{ CV_HLS2BGR,  Unimpl,      MultiToGrey, Unimpl,      Same }
		};

		// see if is handled by opencv
		cv::Mat mat_dst;
		int cv_convert = cv_fmts[(int)get_format()][(int)format];

		if (cv_convert == Same)
		{
			get_opencv()->copyTo(mat_dst);
		}
		else if (cv_convert > 0)
		{
			cvtColor(*this->get_opencv(), mat_dst, cv_convert);
		}
		else if (cv_convert == MultiToGrey)
		{
			IMAGE_PROC_ASSERT(get_opencv()->channels() == 3);

			// collect the intensity channel appropriately for non rgb formats
			int channel = get_intensity_channel(get_format());

			std::vector<cv::Mat> channels;
			cv::split(*get_opencv(), channels);
			channels[channel].copyTo(mat_dst);
		}
		else if (cv_convert == GreyToMulti)
		{
			IMAGE_PROC_ASSERT(get_opencv()->channels() == 1);

			int channel = get_intensity_channel(format);
			auto zero = cv::Scalar(0, 0, 0);
			if (format == Format::Lab)
				zero = cv::Scalar(0, 128, 128);
			mat_dst = cv::Mat(get_height(), get_width(), CV_8UC3, zero);
			std::vector<cv::Mat> channels;
			cv::split(mat_dst, channels);
			get_opencv()->copyTo(channels[channel]);
			cv::merge(channels, mat_dst);
		}
		else
		{
			// remaining conversions are between non-rgb surface types, these we convert
			// by going to rgb and then to the target format, this though adds loss to 
			// the conversion.
			IMAGE_PROC_ASSERT(get_opencv()->channels() == 3);

			// src -> rgb 
			int src2rgb = -1;
			switch (get_format())
			{
			case Format::HLS: src2rgb = CV_HLS2BGR; break;
			case Format::HSV: src2rgb = CV_HSV2BGR; break;
			case Format::Lab: src2rgb = CV_Lab2BGR; break;

			default:
				break;
			}

			// rgb -> dst
			int rgb2dst = -1;
			switch (format)
			{
			case image::Format::Lab: rgb2dst = CV_BGR2Lab; break;
			case image::Format::HSV: rgb2dst = CV_BGR2HSV; break;
			case image::Format::HLS: rgb2dst = CV_BGR2HLS; break;
			
			default:
				break;
			}

			IMAGE_PROC_ASSERT(src2rgb != -1);
			IMAGE_PROC_ASSERT(rgb2dst != -1);

			mat_dst = cv::Mat(get_width(), get_height(), CV_8UC3);
			cvtColor(*get_opencv(), mat_dst, src2rgb);
			cvtColor(mat_dst, mat_dst, rgb2dst);
		}
		dst->set_mat(mat_dst, format);
		return true;		
 	}

	//----------------------------------------------------------------------------

	int get_intensity_channel(image::Format format)
	{
		switch (format)
		{
			case image::Format::Grey: return 0; break;
			case image::Format::HLS: return 1; break;
			case image::Format::HSV: return 2; break;
			case image::Format::Lab: return 0; break;

			default:
				break;
		}
		return -1;
	}

	//----------------------------------------------------------------------------

	void image::resize(int width, int height)
	{
		IMAGE_PROC_ASSERT(width > 0);
		IMAGE_PROC_ASSERT(height > 0);
		
		cv::Mat dst;
		cv::resize(*m_CVMat, dst, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
		set_mat(dst, m_Format);
	}


	//----------------------------------------------------------------------------

	void image::draw(const image* image, int x, int y, int width, int height)
	{
		IMAGE_PROC_ASSERT(x >= 0);
		IMAGE_PROC_ASSERT(y >= 0);
		IMAGE_PROC_ASSERT(width >= 0);
		IMAGE_PROC_ASSERT(height >= 0);
		IMAGE_PROC_ASSERT(image);
		IMAGE_PROC_ASSERT((x + width) <= get_width());
		IMAGE_PROC_ASSERT((y + height) <= get_height());

		if (width == 0 || height == 0)
			return;

		image->m_CVMat->copyTo((*m_CVMat)(cv::Rect(x, y, width, height)));
	}

	//----------------------------------------------------------------------------

	void image::draw_string(const std::string& str,
		int x, int y,
		float font_scale,
		int thickness, Anchor anchor)
	{
		int font_face = cv::FONT_HERSHEY_PLAIN;
		int baseline = 0;
		auto size = cv::getTextSize(str.c_str(), font_face, font_scale, thickness, &baseline);

		cv::Point pos(x, y);
		
		if (anchor == Anchor::Center)
			pos = cv::Point(x - (size.width / 2), y - (size.height / 2));
		else
			pos = cv::Point(x, y + size.height);

		// and draw 
		cv::putText(*m_CVMat, str.c_str(), pos, font_face, font_scale,
			cv::Scalar::all(255), thickness, 8);
	}

	//----------------------------------------------------------------------------

	void image::draw_filled_rect(int x, int y, int w, int h, const cv::Scalar& color)
	{
		cv::rectangle(*get_opencv(), cv::Rect(x, y, w, h), color, CV_FILLED);
	}

	//----------------------------------------------------------------------------

	void image::clear_to_black()
	{
		memset((char*)m_CVMat->data, 0, m_CVMat->step * m_CVMat->rows);
	}


	//----------------------------------------------------------------------------

	void image::clamp_size(int max_size, bool enlarge, Interpolation interp)
	{
		clamp_size(m_CVMat, max_size, enlarge, interp);
	}

	//----------------------------------------------------------------------------

	void image::clamp_size(image* in_dst, int max_size, bool enlarge, Interpolation interp) const
	{
		clamp_size(in_dst->m_CVMat, max_size, enlarge, interp);
	}

	//----------------------------------------------------------------------------

	void image::clamp_size(cv::Mat* in_dst, int max_size, bool enlarge, Interpolation interp) const
	{
		using namespace cv;

		IMAGE_PROC_ASSERT(in_dst);

		// clamp the image to a maximum size whilst maintaining
		// the correct aspect ratio
		int width = get_width();
		int height = get_height();

		if (width == height)
		{
			// square
			if (width > max_size)
				width = height = max_size;
		}
		else if (width > height)
		{
			// landscape 
			if (enlarge || width > max_size)
			{
				float scale = (float)max_size / width;
				width = max_size;
				height = (int)(scale * height);
			}
		}
		else
		{
			// portrait
			if (enlarge || height > max_size)
			{
				float scale = (float)max_size / height;
				height = max_size;
				width = (int)(scale * width);
			}
		}

		if (get_width() != width || get_height() != height)
		{
			int cvInterp = INTER_NEAREST;
			switch (interp)
			{
			case Interpolation::Cubic: cvInterp = INTER_CUBIC; break;
			case Interpolation::Linear: cvInterp = INTER_LINEAR; break;
			default:
				break;	
			}

			cv::Size new_size = Size(width, height);
			cv::resize(*m_CVMat, *in_dst, new_size, 0, 0, cvInterp);
		}
		else
		{
			m_CVMat->copyTo(*in_dst);
		}
	}

	//----------------------------------------------------------------------------

	void image::remap_to_palette(const cv::Vec3b* palette, size_t num_entries)
	{
		using namespace cv;

		IMAGE_PROC_ASSERT(palette);
		IMAGE_PROC_ASSERT(num_entries > 0);

		// just a brute force linear search
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				auto& pixel = m_CVMat->at<cv::Vec3b>(y, x);

				float min_dist = std::numeric_limits<float>::max();
				Vec3b closest_clr = Vec3b::all(0);
				for (size_t p = 0; p < num_entries; ++p)
				{
					auto clr = palette[p];
					auto vec = Vec3f(clr) - Vec3f(pixel);
					float dist = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
					if (dist < min_dist)
					{
						min_dist = dist;
						closest_clr = clr;
					}
				}

				pixel = closest_clr;
			}
		}
	}

	//----------------------------------------------------------------------------

	size_t image::get_detailed_palette_information(detailed_palette& out_pal)
	{
		using namespace cv;

		std::map<uint32_t, size_t> lookup;

		const Mat& src = *get_opencv();
		const int src_width = src.size().width;
		const int src_height = src.size().height;

		for (int y = 0; y < src_height; ++y)
		{
			for (int x = 0; x < src_width; ++x)
			{
				Vec3b val = src.at<Vec3b>(Point(x, y));
				uint32_t key = val[0] | (val[1] << 8) | (val[2] << 16);
				auto item = lookup.find(key);
				if (item == lookup.end())
				{
					palette_entry entry;
					entry.color[0] = val[0];
					entry.color[1] = val[1];
					entry.color[2] = val[2];
					++entry.num_pixels;
					out_pal.emplace_back(entry);
					lookup.insert(std::make_pair(key, out_pal.size() - 1));
				}
				else
				{
					++out_pal[item->second].num_pixels;
				}
			}
		}

		for (auto& entry : out_pal)
		{
			entry.coverage = float(entry.num_pixels) / (get_width() * get_height()) * 100;
		}

		return out_pal.size();
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
