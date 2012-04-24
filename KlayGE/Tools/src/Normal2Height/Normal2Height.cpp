#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/BlockCompression.hpp>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
namespace
{
	using namespace KlayGE;

	void CreateDDM(std::vector<float2>& ddm, std::vector<float3> const & normal_map)
	{
		ddm.resize(normal_map.size());
		for (size_t i = 0; i < normal_map.size(); ++ i)
		{
			float3 n = normal_map[i];
			n.z() = std::max(n.z(), 0.1f);
			ddm[i].x() = n.x() / n.z();
			ddm[i].y() = n.y() / n.z();
		}
	}

	void AccumulateDDM(std::vector<float>& height_map, std::vector<float2> const & ddm, uint32_t width, uint32_t height, int directions, int rings)
	{
		float step = 2 * PI / directions;

		std::vector<float2> tmp_hm(ddm.size(), float2(0, 0));
		for (size_t i = 0; i < ddm.size(); ++ i)
		{
			int y = i / width;
			int x = i - y * width;

			int n = 0;
			for (int j = 0; j < directions; ++ j)
			{
				float angle = j * step;
				float dx, dy;
				MathLib::sincos(angle, dx, dy);
				for (int k = 1; k < rings; ++ k)
				{
					float2 delta(dx * k, dy * k);
					int offset_x = static_cast<int>(delta.x() + 0.5f);
					int offset_y = static_cast<int>(delta.y() + 0.5f);
					int sample_x = x + offset_x;
					int sample_y = y + offset_y;
					if ((sample_x >= 0) && (sample_x < static_cast<int>(width)) && (sample_y >= 0) && (sample_y < static_cast<int>(height)))
					{
						tmp_hm[i] = tmp_hm[sample_y * width + sample_x] + ddm[sample_y * width + sample_x] * delta;
						++ n;
					}
				}
			}

			if (n > 0)
			{
				tmp_hm[i] /= n;
			}
		}

		height_map.resize(ddm.size());
		for (size_t i = 0; i < ddm.size(); ++ i)
		{
			height_map[i] = MathLib::length(tmp_hm[i]);
		}
	}

	void CreateHeightMap(std::string const & in_file, std::string const & out_file)
	{
		Texture::TextureType type;
		uint32_t width, height, depth;
		uint32_t num_mipmaps;
		uint32_t array_size;
		ElementFormat format;
		std::vector<ElementInitData> in_data;
		std::vector<uint8_t> in_data_block;
		LoadTexture(in_file, type, width, height, depth, num_mipmaps, array_size, format, in_data, in_data_block);

		if ((Texture::TT_2D == type) && ((EF_ABGR8 == format) || (EF_ARGB8 == format) || (EF_BC5 == format)))
		{
			uint32_t the_width = width;
			uint32_t the_height = height;

			std::vector<std::vector<float> > heights(in_data.size());
			for (size_t i = 0; i < in_data.size(); ++ i)
			{
				uint8_t const * p = static_cast<uint8_t const *>(in_data[i].data);

				std::vector<float3> normals(the_width * the_height);
				if (EF_BC5 == format)
				{
					std::vector<uint8_t> gr(std::max(the_width, 4U) * std::max(the_height, 4U) * 2);
					DecodeBC5(&gr[0], the_width * 2, p, the_width, the_height);
					for (uint32_t y = 0; y < the_height; ++ y)
					{
						for (uint32_t x = 0; x < the_width; ++ x)
						{
							float nx = gr[(y * the_width + x) * 2 + 0] / 255.0f * 2 - 1;
							float ny = gr[(y * the_width + x) * 2 + 1] / 255.0f * 2 - 1;
							normals[y * the_width + x].x() = nx;
							normals[y * the_width + x].y() = ny;
							normals[y * the_width + x].z() = sqrt(std::max(0.0f, 1 - nx * nx - ny * ny));
						}
					}
				}
				else
				{
					for (uint32_t y = 0; y < the_height; ++ y)
					{
						for (uint32_t x = 0; x < the_width; ++ x)
						{
							normals[y * the_width + x].x() = p[y * in_data[i].row_pitch + x * 4 + 0] / 255.0f * 2 - 1;
							normals[y * the_width + x].y() = p[y * in_data[i].row_pitch + x * 4 + 1] / 255.0f * 2 - 1;
							normals[y * the_width + x].z() = p[y * in_data[i].row_pitch + x * 4 + 2] / 255.0f * 2 - 1;
						}
					}
					if (EF_ARGB8 == format)
					{
						for (uint32_t y = 0; y < the_height; ++ y)
						{
							for (uint32_t x = 0; x < the_width; ++ x)
							{
								std::swap(normals[y * the_width + x].x(), normals[y * the_width + x].z());
							}
						}
					}
				}

				std::vector<float2> ddm;
				CreateDDM(ddm, normals);

				std::vector<float> acc_heights;
				AccumulateDDM(acc_heights, ddm, the_width, the_height, 36, 5);

				heights[i].resize(acc_heights.size());
				for (uint32_t y = 0; y < the_height; ++ y)
				{
					for (uint32_t x = 0; x < the_width; ++ x)
					{
						heights[i][y * the_width + x] = acc_heights[y * the_width + x];
					}
				}

				the_width = (the_width + 1) / 2;
				the_height = (the_height + 1) / 2;
			}

			float min_height = +1e10f;
			float max_height = -1e10f;
			for (size_t i = 0; i < heights.size(); ++ i)
			{
				for (size_t j = 0; j < heights[i].size(); ++ j)
				{
					min_height = std::min(min_height, heights[i][j]);
					max_height = std::max(max_height, heights[i][j]);
				}
			}
			if (max_height - min_height > 1e-6f)
			{
				for (size_t i = 0; i < heights.size(); ++ i)
				{
					for (size_t j = 0; j < heights[i].size(); ++ j)
					{
						heights[i][j] = (heights[i][j] - min_height) / (max_height - min_height) * 0.5f + 0.5f;
					}
				}
			}

			std::vector<uint8_t> data_block;
			std::vector<size_t> base(in_data.size());
			for (size_t i = 0; i < heights.size(); ++ i)
			{
				base[i] = data_block.size();
				data_block.resize(data_block.size() + heights[i].size());

				for (size_t j = 0; j < heights[i].size(); ++ j)
				{
					data_block[base[i] + j] = static_cast<uint8_t>(MathLib::clamp(static_cast<int>(heights[i][j] * 255 + 0.5f), 0, 255));
				}
			}

			the_width = width;
			the_height = height;
			std::vector<ElementInitData> heights_data(in_data.size());
			for (size_t i = 0; i < heights.size(); ++ i)
			{
				heights_data[i].data = &data_block[base[i]];
				heights_data[i].row_pitch = the_width;
				heights_data[i].slice_pitch = the_width * the_height;

				the_width = (the_width + 1) / 2;
				the_height = (the_height + 1) / 2;
			}

			SaveTexture(out_file, type, width, height, depth, num_mipmaps, array_size, EF_R8, heights_data);
		}
		else
		{
			cout << "Unsupported texture format" << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	using namespace KlayGE;

	if (argc != 3)
	{
		cout << "Usage: Normal2Height xxx.dds yyy.dds" << endl;
		return 1;
	}

	CreateHeightMap(argv[1], argv[2]);

	cout << "Height map is saved to " << argv[2] << endl;

	return 0;
}