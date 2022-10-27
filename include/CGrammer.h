#pragma once
#include<string>
#include<map>
#include<vector>
#include<set>
#include<assert.h>
#include<iostream>
typedef std::pair<char, std::string> EXPR;
class CGrammer
	//注意，0作为空使用
{
private:
	std::vector<EXPR> Rules;
	std::vector<std::string>exprs[256];
	std::set<char> Fir[256], Fol[256];
	std::set<char> Vn;
	std::set<char> Vt;
public:
	CGrammer() { };
	~CGrammer() {};
	CGrammer(std::vector<std::string> exprs) {
		for (auto expr : exprs) {
			assert(expr[1] == ' ');
			add_rules(expr[0], expr.substr(2, -1));
		}
	}
	std::pair<bool, bool> static Merge(std::set<char>& x, std::set<char>& y, int without = -1)
	{
		bool ishave = false;
		bool update = false;
		for (auto w : y)
			if (w != without)
				update |= x.insert(w).second;
			else ishave = true;
		return std::make_pair(ishave, update);
	}
	bool static in_Vn(char x) {
		return 'A' <= x && x <= 'Z';
	}
	void add_rules(char u, std::string s)
	{
		EXPR expr = std::make_pair(u, s);
		Rules.push_back(expr);
		exprs[u].push_back(s);
		Vn.insert(u);
		for (auto c : s)
			if (!in_Vn(c))
				Vt.insert(c);
	}
	void input() {
		std::string x, s;
		for (;;) {
			std::cin >> x;
			if (!in_Vn(x[0]))
				break;
			std::cin >> s;
			add_rules(x[0], s);
		}
	}
	std::vector<std::string>& getExprs(char u) {
		return exprs[u];
	}
	std::set<char>& getVns() {
		return Vn;
	}
	std::set<char>& getVts() {
		return Vt;
	}
	std::set<char>& getFol(char u) {
		return Fol[u];
	}
	std::set<char>& getFir(char u) {
		return Fir[u];
	}
	std::set<char> Get_First(std::string s)
	{
		std::set<char>fir; int i = 0;
		for (; i < s.size(); ++i)
		{
			if (in_Vn(s[i])) {
				bool can_empty = Merge(fir, Fir[s[i]], '0').first;
				if (!can_empty)
					break;
				else fir.insert('0');
			}
			else {
				fir.insert(s[i]);
				break;
			}
		}
		if (i == s.size())
			fir.insert('0');
		return fir;
	}
	bool Get_Follow(char x, std::string s)
	{
		bool update = false;
		if (x == 'S') Fol[x].insert('$');
		for (int i = 0; i < s.size(); ++i)
		{
			if (in_Vn(s[i]))
			{
				auto r = Merge(Fol[s[i]], Get_First(s.substr(i + 1, -1)), '0');
				bool can_empty = r.first;
				update |= r.second;
				if (can_empty)
					update |= Merge(Fol[s[i]], Fol[x]).second;
			}
		}
		return update;
	}
	void initialize()
	{
		bool update;
		for (;;) {
			update = false;
			for (int x = Rules.size() - 1; x >= 0; --x)
				update |= Merge(Fir[Rules[x].first], Get_First(Rules[x].second)).second;
			if (!update)
				break;
		}
		for (;;) {
			update = false;
			for (int x = 0; x < Rules.size(); ++x)
				update |= Get_Follow(Rules[x].first, Rules[x].second);
			if (!update)
				break;
		}
	}
	void showinfo()
	{
		printf("output cgrammar info-------------\n");
		for(auto i : Vn)
		{
			assert(in_Vn(i));
			printf("First(%c)={", char(i));
			for (auto x : Fir[i])
				printf("%c,", x);
			puts("}");
		}
		for (auto i : Vn)
		{
			assert(in_Vn(i));
			printf("Follow(%c)={", char(i));
			for (auto x : Fol[i])
				printf("%c,", x);
			puts("}");
		}
	}
};

