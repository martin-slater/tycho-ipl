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


#ifdef _MSC_VER
#pragma once
#endif  // _MSC_VER

#ifndef IMAGE_H_8CDC3D04_53A5_470E_A193_35127CF91399
#define IMAGE_H_8CDC3D04_53A5_470E_A193_35127CF91399

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "image_processing_abi.h"
#include "exception.h"

//TODO: remove this from the header so all opencv is behind a compiler firewall
#include "opencv2/core/matx.hpp"

#include <vector>

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{   
namespace image_processing
{

	/// thrown if there is a problem reading an image file
	class image_read_error : public exception {
	public:
		image_read_error(const char *path) :
			m_Path(path)
		{}

		const char* what() const noexcept override {
			static std::array<char, 256> buffer;
			sprintf(&buffer[0], "Failed to read '%s'", m_Path.c_str());
			return buffer.data();
		}

	private:
		std::string m_Path;
	};

	struct palette_entry
	{
		using map_list = std::vector<int>;

		cv::Vec3b color;	// Entry color
		float     coverage;	// Normalised pixel coverage in range [0,100]
		map_list   mappings;	// list of other palette entries this maps to.
		size_t    index;	//  index in the palette array
		bool      merged = false;
		size_t    num_pixels = 0;
	};

	using detailed_palette = std::vector<palette_entry>;

	//----------------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------------
	class TYCHO_IMAGEPROCESSING_ABI image
	{
	public:
		enum class Format
		{
			RGB,
			Lab,
			Grey,
			HSV,
			HLS,
			Count
		};

		enum class Anchor
		{
			Center,
			TopLeft
		};

		enum class Interpolation
		{
			Nearest,
			Linear,
			Cubic
		};

	public:
		/// Initialise empty image
		image(Format, int width, int height);

		/// Copy constructor
		image(const image& img);

		/// Load from file 
		image(const char *);

		/// Load from file 
		image(const std::string&);

		/// Default constructor
		image() = default;

		/// Destructor
		virtual ~image();

		/// Clone this image
		image* clone() const; 
		
		/// Get underlying OpenCV image
		cv::Mat* get_opencv();

		/// Get underlying OpenCV image
		const cv::Mat* get_opencv() const
		{
			return m_CVMat;
		}

		/// Returns true if the format is the type supplied
		bool format_is(Format fmt) const 
		{
			return m_Format == fmt; 
		}

		/// Write the image to disk
		bool write_to_file(const char*) const;

		/// Write the image to disk
		bool write_to_file(const std::string& ) const;

		/// Returns the width of the image
		int get_width() const;

		/// Returns the height of the image
		int get_height() const;

		/// Get the image format. \see Image::Format
		Format get_format() const;

		/// Convert image to another format.
		bool convert_to(image* dst, Format format);

		/// Set the underlying open cv Mat
		void set_mat(const cv::Mat& mat, Format fmt);

		/// Get the path the image was originally loaded from
		const std::string& get_source_path() const;

		/// Resize the image
		void resize(int width, int height);

		/// Draw the passed image into this image
		void draw(const image* image, int x, int y, int width, int height);

		/// Draw a text string to the image anchored at the given position.
		void draw_string(const std::string& str,
			int x, int y,
			float font_scale,
			int thickness,
			Anchor anchor);

		/// Draw a rectangle to the image
		void draw_filled_rect(int x, int y, int w, int h, const cv::Scalar& color);

		/// Clear the image to black
		void clear_to_black();

		/// Clamp an image to a given size. If the image is less than the given
		/// size on both axis it is left untouched otherwise it will be scaled
		/// so the longest edge is the given size whilst maintaining the correct
		/// aspect ratio.
		void clamp_size(image* in_dst, int max_size, bool enlarge, Interpolation) const;

		/// As clamp_size but modifies current image
		void clamp_size(int max_size, bool enlarge, Interpolation);

		/// Remaps all the pixels in the image to closest match in the passed
		/// palette
		void remap_to_palette(const cv::Vec3b* palette, size_t num_entries);

		/// Returns detail information about colors used in the image
		size_t get_detailed_palette_information(detailed_palette& out_pal);

	private:
		cv::Mat* m_CVMat{nullptr};
		Format   m_Format;
		std::string m_SourcePath;

		// non-copyable
		image& operator=(const image&) = delete;

		void clamp_size(cv::Mat* in_dst, int max_size, bool enlarge, Interpolation) const;
	};


	int get_intensity_channel(image::Format format);

	using image_ptr = std::shared_ptr < image > ;
	using image_ptr_list = std::vector < image_ptr > ;
	
} // end namespace
} // end namespace

#endif // IMAGE_H_8CDC3D04_53A5_470E_A193_35127CF91399
