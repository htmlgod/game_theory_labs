#include <random>
#include <optional>
#include <set>

#include <matrix_operations.hpp>

using matrix = std::vector<std::vector<size_t>>;
using vec = std::vector<size_t>;

size_t get_min_element_index(const std::vector<size_t>& vec) {
    return std::distance(vec.begin(), std::min_element(vec.begin(), vec.end()));
}

size_t get_max_element_index(const std::vector<size_t>& vec) {
    return std::distance(vec.begin(), std::max_element(vec.begin(), vec.end()));
}

matrix get_subgame(const matrix& A, const std::set<size_t>& J) {
	auto tmp = get_transposed_matrix(A);
	matrix res;
	for (auto col : J) {
		res.push_back(tmp[col]);
	}
	return res;
}

matrix reduce_game_matrix(const matrix& gm) {
	auto tmp = get_transposed_matrix(gm);
	while (tmp[0].size() != tmp.size()) {
		// cols > rows
		if (tmp[0].size() > tmp.size()) {
			std::optional<size_t> index_for_deletion;
			bool found = true;
			for (size_t i = 0; i < tmp.size(); ++i) { // идем по строчкам
				for (size_t r = 0; r < tmp.size(); ++r) { 
					for (size_t j = 0; j < tmp[0].size(); ++j) {
						if (i != r) {
							if (tmp[i][j] > tmp[r][j]) {
								found = false;
								break;
							}
						}
					}
					if (found) {
						index_for_deletion = i;
						break;
					}
				}
				if (found) {
					break;
				}
			}
			if (index_for_deletion) {
				tmp.erase(tmp.begin() + index_for_deletion.value());
			}
			tmp = get_transposed_matrix(tmp);
		}
		// rows > cols
		else if (tmp[0].size() < tmp.size()) {
			tmp = get_transposed_matrix(tmp);
			std::optional<size_t> index_for_deletion;
			bool found = true;
			for (size_t i = 0; i < tmp.size(); ++i) { // идем по строчкам
				for (size_t r = 0; r < tmp.size(); ++r) { 
					for (size_t j = 0; j < tmp[0].size(); ++j) {
						if (i != r) {
							if (tmp[i][j] < tmp[r][j]) {
								found = false;
								break;
							}
						}
					}
					if (found) {
						index_for_deletion = i;
						break;
					}
				}
				if (found) {
					break;
				}
			}
			if (index_for_deletion) {
				tmp.erase(tmp.begin() + index_for_deletion.value());
			}
		}
	}
	return tmp;
}

std::vector<float> solve_2x2subgame(const matrix& m) {
	return {

	};

}

auto main() -> int {
	// matrix A = {
	// 	{8,1,17,8,1},
	// 	{12,6,11,10,16},
	// 	{4,19,11,15,2},
	// 	{17,19,6,17,16}
	// };
	matrix A = {
		{2,1,3},
		{3,0,1},
		{1,2,1}
	};
	print_matrix(A);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> distrib(0, A.size() - 1);


	// iter 0: select random x strat
	std::set<size_t> J;
	auto i0_strat = distrib(gen);
	auto c0_vec = A[i0_strat];
	auto x0_vec = vec(A.size(), 0);
	x0_vec[i0_strat] = 1;

	print_vector(x0_vec, 3);
	print_vector(c0_vec, 3);
	J.insert(get_min_element_index(c0_vec));
	matrix C;
	C.push_back(c0_vec);

	// iter 1
	auto sub_game = get_subgame(A, J); 
	print_matrix(sub_game);
	auto i_strat = get_max_element_index(sub_game[0]);
	auto x1_vec = vec(A.size(), 0);
	x1_vec[i_strat] = 1;
	auto c1_vec = A[i_strat];
	print_vector(x1_vec, 3);
	print_vector(c1_vec, 3);
	C.push_back(c1_vec);
	//auto reduced = reduce_game_matrix(C);
	print_matrix(C);
}
