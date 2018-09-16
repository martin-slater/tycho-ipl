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
#include "contact_sheet.h"
#include "functions/common.h"
#include "result_set.h"
#include "image.h"

//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

namespace tycho
{
namespace image_processing
{

	//----------------------------------------------------------------------------

	contact_sheet::contact_sheet()
	{

	}

	//----------------------------------------------------------------------------

	void contact_sheet::auto_build(
		const std::string& /*title*/, 
		const result_matrix& results,
		const std::string& output_path,
		const runtime::session_options::file_list& input_files)
	{
		bool draw_originals = true;
		image_ptr_list thumbnails;

		if (input_files.size() == 1)
			draw_originals = false;

		if (results.get_num_nodes() == 0 )
			return;
		
		if (results.num_dimensions() == 1 || results.num_dimensions() == 2)
		{
			int max_image_width = -1;
			int max_image_height = -1;

			const size_t dim1 = (results.num_dimensions() == 1) ? 1 : results.get_dimension_size(1);
			for (size_t y = 0; y < dim1; ++y)
			{
				for (size_t x = 0; x < results.get_dimension_size(0); ++x)
				{
					file_list_node* img_list = nullptr;
					
					if (results.num_dimensions() == 1)
						img_list = (file_list_node*)results.get_node({ x }).get();
					else
						img_list = (file_list_node*)results.get_node({ x, y }).get();
					
					IMAGE_PROC_ASSERT(img_list->get_type() == node_base::Type::FileList);
					IMAGE_PROC_ASSERT(img_list->get_entries().size() == 1);

					image_ptr img(new image(img_list->get_entries()[0].Path));
					img->clamp_size(ImageSize, false, image::Interpolation::Cubic);

					thumbnails.push_back(img);
					img_list->set_user_data(img.get());

					max_image_height = std::max<int>(img->get_height(), max_image_height);
					max_image_width = std::max<int>(img->get_width(), max_image_width);
				}
			}

			size_t num_images_wide = 0;
			size_t num_images_high = 0;


			if (results.num_dimensions() == 1)
			{
				num_images_wide = results.get_dimension_size(0);
				num_images_high = 1;
			}
			else
			{
				num_images_wide = results.get_dimension_size(1);
				num_images_high = results.get_dimension_size(0);
			}

			if (draw_originals)
				++num_images_wide;

			// create single image to compose all images on to
			size_t total_width = num_images_wide * max_image_width + 
								2 * ImageBorder + (num_images_wide - 1) * ImageBorder;
			size_t total_height = num_images_high * (max_image_height + InfoRectHeight) + 
								ImageTopBorder + ImageBorder + (num_images_high - 1) * ImageBorder;

			image contact_sheet(image::Format::RGB, total_width, total_height);
			contact_sheet.clear_to_black();

			// write column headers
			for (size_t i = 0; i < num_images_wide; ++i)
			{
				std::string text;
				if (i == 0 && draw_originals)
				{
					text = "original";
				}
				else
				{
// 					const size_t node_idx = draw_originals ? i-1 : i;
// 					auto node = (FileListNode*)results.get_node({ 0, node_idx }).get();
// 					text = node->GetEntries()[0].Annotation;
					continue;
				}

				int y = ImageTopBorder / 2;
				int x = max_image_width / 2 + ImageBorder + (i * (max_image_width + ImageBorder));

				contact_sheet.draw_string(text, x, y, FontScale, FontThickness, image::Anchor::Center);
			}

			// write all images to the contact sheet
			int y_off = ImageTopBorder;
			for (size_t y = 0; y < num_images_high; ++y)
			{
				int x_off = ImageBorder;

				if (draw_originals)
				{
					image_ptr orig(new image(input_files[y]));

					if (max_image_width > max_image_height)
						orig->clamp_size(max_image_width, false, image::Interpolation::Cubic);
					else
						orig->clamp_size(max_image_height, false, image::Interpolation::Cubic);

					int cx_off = (max_image_width - orig->get_width()) / 2;
					int cy_off = (max_image_height - orig->get_height()) / 2;

					contact_sheet.draw(orig.get(), x_off + cx_off, y_off + cy_off, orig->get_width(), orig->get_height());


					int info_x = x_off + cx_off;
					int info_y = y_off + cy_off + orig->get_height();// - InfoRectHeight;
					contact_sheet.draw_filled_rect(info_x, info_y,
						orig->get_width(), InfoRectHeight, cv::Scalar(96, 96, 96, 0));

					contact_sheet.draw_string(
						"Original Image",
						info_x + InfoRectPadding,
						info_y + InfoRectPadding, FontScale, FontThickness, image::Anchor::TopLeft);

					x_off += max_image_width + ImageBorder;
				}
				for (size_t x = 0; 
					x < (draw_originals ? num_images_wide - 1 : num_images_wide); 
					++x)
				{
					const file_list_node* node = nullptr;
					if (results.num_dimensions() == 1)
						node = (const file_list_node*)results.get_node({ x }).get();
					else
						node = (const file_list_node*)results.get_node({ y, x }).get();
					auto img = (image*)node->get_user_data();

					int cx_off = (max_image_width - img->get_width()) / 2;
					int cy_off = (max_image_height - img->get_height()) / 2;

					contact_sheet.draw(img, x_off + cx_off, y_off + cy_off, img->get_width(), img->get_height());

					int info_x = x_off + cx_off;
					int info_y = y_off + cy_off + img->get_height();// -InfoRectHeight;
					contact_sheet.draw_filled_rect(info_x, info_y,
						img->get_width(), InfoRectHeight, cv::Scalar(96, 96, 96, 0));

					contact_sheet.draw_string(
						node->get_entries()[0].Annotation, 
						info_x + InfoRectPadding, 
						info_y + InfoRectPadding, FontScale, FontThickness, image::Anchor::TopLeft);

					x_off += max_image_width + ImageBorder;
				}
 				y_off += max_image_height + ImageBorder;
			}

			// write the contact sheet to disk
			contact_sheet.write_to_file(output_path);
		}
		else if (results.num_dimensions() == 3)
		{
			// write a layer per sheet
		}
		
	}

	//----------------------------------------------------------------------------

} // end namespace
} // end namespace
