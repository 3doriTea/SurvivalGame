#pragma once

namespace GameBase
{
	static inline std::string ReferencePrinter(const std::vector<std::pair<size_t, std::vector<size_t>>>& _mapping)
	{
		std::stringstream ss{};

		// row head
		ss << "+ ";
		std::vector<size_t> keys(_mapping.size());
		size_t i{};
		for (const auto& [key, _] : _mapping)
		{
			keys.at(i) = key;
			ss << key << ",";
			i++;
		}
		ss << std::endl;

		// column
		size_t line{ 1 };

		for (const auto& [index, refers] : _mapping)
		{
			ss << index << " ";

			for (int i = 0; i < keys.size(); i++)
			{
				if (std::find(refers.begin(), refers.end(), keys.at(i)) != refers.end())
				{
					ss << "* ";
				}
				else if (line - 1 == i)
				{
					ss << "+ ";
				}
				else
				{
					ss << "  ";
				}
			}

			ss << std::endl;
			line++;
		}

		return ss.str();
	}
}
