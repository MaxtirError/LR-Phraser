#pragma once
#include"CGrammer.h"
#include<queue>
class ItemSet
{
private:
	CGrammer* grammer;
	std::set<EXPR> exprs;
	int index;
	std::vector<std::pair<int, char>>Moves;
public:
	ItemSet(CGrammer* _grammer = NULL, int _index = -1) { grammer = _grammer; index = _index; };
	~ItemSet() {};
	bool operator < (ItemSet B) const {
		return exprs < B.exprs;
	}
	bool empty() { return exprs.empty(); }
	void add_expr(char x, std::string s) {
		exprs.insert(std::make_pair(x, s));
	}
	int Get_index() { return index; }
	void Set_index(int _index) { index = _index; }
	void Showinfo() {
		printf("items id:%d\n", index);
		puts("Item Content:");
		for (auto x : exprs) {
			std::cout << x.first << "->" << x.second<<std::endl;
		}
		puts("Move Condition:");
		for (auto x : Moves) {
			printf("Accept %c To %d\n", x.second, x.first);
		}
	}
	void add_Moves(int To, char E) {
		Moves.push_back(std::make_pair(To, E));
	}

	void GetClosure();
	ItemSet GoTo(char x) const;
}; 
class CPraser
{
private:
	CGrammer grammer;
	std::vector<ItemSet> items;
	std::map < ItemSet, int> items_id;
public:
	CPraser() { };
	~CPraser() { };
	void input() {
		grammer.input();
		grammer.initialize();
		grammer.showinfo();
	}
	void SLR_Prase();
	void ShowInfo();
};

