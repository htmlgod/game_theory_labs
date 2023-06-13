#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <set>

std::random_device rd; 
auto seed = rd();//2575149780; 710820976
std::ostream& foo = std::cout << seed << std::endl;
std::mt19937 gen(seed);

auto find_maximums(const std::vector<int>& v) {
   std::vector<size_t> indexes;

   for (auto it_max = std::max_element(v.begin(), v.end()); it_max != v.end();
        it_max = std::find(it_max+1, v.end(), *it_max))
   {
      auto index = std::distance(v.begin(), it_max);
      indexes.push_back(index);
   }

   return indexes;
}

auto find_maximums(const std::vector<std::vector<int>>& v, size_t player) {
    std::vector<size_t> indexes;
    for (auto it_max = std::max_element(v.begin(), v.end(), 
                [player](const std::vector<int>& a, const std::vector<int>& b) {
                return a[player] < b[player];
                }); it_max != v.end();
        //it_max = std::find(it_max+1, v.end(), *it_max))
        it_max = std::find_if(it_max + 1, v.end(), 
                [player, it_max](const std::vector<int>& a) { return a[player] == (*it_max)[player]; }))
    {
        auto index = std::distance(v.begin(), it_max);
        indexes.push_back(index);
    }
    return indexes;
}

auto gen_rand_int(int lower_bound, int upper_bound) {
    std::uniform_int_distribution<> distrib(lower_bound, upper_bound);
    return distrib(gen);
}

struct node {
    size_t vertex_num;
    node* parent;
    std::vector<node*> leaves;
    node(size_t num) : vertex_num(num) {}
    ~node() {}
    std::vector<int> players_wins;
    std::set<std::vector<int>> paths;
    size_t player;
};

size_t compute_vertexes_amount(size_t depth, std::vector<size_t> strategies_amount) {
    size_t amount = 0;
    size_t prev_val = 1;
    auto total_players = strategies_amount.size();
    for (size_t i = 0; i < depth; ++i) {
        auto val = prev_val * strategies_amount[i % total_players];
        amount += val;
        prev_val = val;
    }
    return amount;
}

class graph {
public:
    graph(size_t depth = 6, std::vector<size_t>&& strategies_amount = {2,3}, int lower_bound = 0, int upper_bound = 20, size_t players_amount = 2) {
        root = new node(0);
        root->parent = nullptr;
        root->player = 0;
        this->total_vertexes = compute_vertexes_amount(depth, strategies_amount);
        this->depth = depth;
        this->strategies_amount = strategies_amount;
        this->total_players = players_amount;
        this->lower_bound = lower_bound;
        this->upper_bound = upper_bound;
        nodes_by_depth.resize(depth+1);
        create_leaves(root);
    }
    auto gen_wins() {
        std::vector<int> res;
        for (size_t i = 0; i < total_players; ++i) {
            res.push_back(gen_rand_int(lower_bound, upper_bound));
        }
        return res;
    }
    void create_leaves(node* leaf, size_t current_depth = 1) {
        if (index == total_vertexes or current_depth - 1 == depth) {
            leaf->paths.insert(gen_wins());
            return;
        }
        nodes_by_depth[current_depth].push_back(leaf);
        leaf->leaves.resize(strategies_amount[(current_depth - 1) % total_players]);
        auto leaves_size = strategies_amount[(current_depth - 1) % total_players];
        for (auto& l : leaf->leaves) {
            l = new node(index++);
            l->parent = leaf;
            l->player = (l->parent->player+1) % total_players;
            create_leaves(l, current_depth+1);
        }
    }
    void solve_by_backward_induction() {
        for (auto it = nodes_by_depth.rbegin(); it != nodes_by_depth.rend() - 1; ++it) {
            make_reverse_step(*it);
        }
        root->paths = {};
        for (auto& p : root->leaves[0]->paths) {
            for (auto& pp : root->leaves[1]->paths) {
                if (p[0] > pp[0]) {
                    root->paths.insert(p);
                    std::cout << "adding " << p[0] << "\n";
                }
                else if (p[0] < pp[0]) {
                    root->paths.insert(pp);
                    std::cout << "adding " << pp[0] << "\n";
                }
                else if (p[0] == pp[0]) {
                    root->paths.insert(pp);
                    root->paths.insert(p);
                    std::cout << "adding both " << pp[0] << " and " << p[0] << "\n";
                }
            }
        }
    }
    void make_reverse_step(std::vector<node*>& level) {
        for (auto& leaf : level) {
            auto total_strats = strategies_amount[leaf->player]; 
            auto leaf_size = leaf->leaves.size();
            auto cur_player = leaf->player;

            std::vector<int> max_from_each_path;
            for (auto& l : leaf->leaves) { 
                std::vector<int> tmp;
                for (auto& p : l->paths) {
                    // print_path_elem(p);
                    tmp.push_back(p[cur_player]); 
                }
                max_from_each_path.push_back(*std::max_element(tmp.begin(), tmp.end()));
            }
            // print_path_elem(max_from_each_path);
            auto indexes = find_maximums(max_from_each_path);
            for (auto i : indexes) {
                for (auto p : leaf->leaves[i]->paths) {
                    leaf->paths.insert(p);
                }
            }
        }
    }
    void print_path_elem(std::vector<int> p) {
            std::cout << "[";
            for (auto el : p) {
                std::cout << el << " ";
            }
            std::cout << "] ";
    }

    void write_win(std::ostream& os, const std::vector<int>& w) {
        os << "(";
        for (size_t i = 0; i < w.size(); ++i) {
            os <<  w[i];
            if (i + 1 < w.size()) {
                os << ", ";
            }
        }
        os << ")";
    }
    void write_node(const node* n, std::ostream& os) {
        if (n == nullptr) return;
        if (!n->leaves.empty()) {
            for (const node* l : n->leaves) {
                bool printed = false;
                for (const auto& [k,v] : path_color) {
                    if (std::find(l->paths.begin(), l->paths.end(), k) != l->paths.end() and std::find(n->paths.begin(), n->paths.end(), k) != n->paths.end()) {
                        os << '\t' << n->vertex_num << " -> " << l->vertex_num;
                        os << " [style=bold, label=\"Pl. " << n->player;
                        os << "\";" << colors[v] << "];\n";
                        printed = true;
                    }
                }
                if (!printed) {
                    os << '\t' << n->vertex_num << " -> " << l->vertex_num;
                    os << " [style=bold, label=\"Pl. " << n->player;
                    os << "\"];\n";
                }
                write_node(l, os);
            }
        }
        if (!n->paths.empty()) {
            os << '\t' << n->vertex_num << " [label=\"" << n->vertex_num << "\\n";
            //for (size_t i = 0; i < n->paths.size(); ++i) {
            for (auto it = n->paths.begin(); it != n->paths.end();) {
                write_win(os, *it);
                if (++it != n->paths.end()) {
                    os << "\\n";
                }
            }
            os << "\"];";
        }
    }
    void write_graph_to_file() {
        std::ofstream of("out.gv");
        of << "digraph G {\n";
        //of << "\tsize= \"120,8\"\n";
        //of << "\tdpi= \"96\"\n";
        int i = 0;
        for (const auto& p : root->paths) {
            if (!path_color.count(p)) path_color[p] = i++;
        }
        write_node(root, of);
        of << "}";
    }

    void delete_node(node* p) {
        if (p == nullptr) return;
        if (!p->leaves.empty()) {
            for (auto l : p->leaves) {
                delete_node(l);
            }
        }
        delete p;
    }

    ~graph() {
        if (root != nullptr) {
            delete_node(root);
        }
    }

private:
    const char* const colors[4] = {"color=red", "color=green", "color=purple", "color=blue" };
    std::map<std::vector<int>, int> path_color;
    node* root = nullptr;
    size_t total_vertexes = 0;
    size_t total_players;
    size_t depth;
    size_t index = 1;
    int upper_bound;
    int lower_bound;
    std::vector<size_t> strategies_amount;
    std::vector<node*> starting_leaves;
    std::vector<std::vector<node*>> nodes_by_depth;
};

auto main() -> int {
    graph g{};
    g.solve_by_backward_induction();
    g.write_graph_to_file();
    return 0;
}
