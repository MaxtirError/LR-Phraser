#pragma once
#include<string>
#include<map>
#include<vector>
#include<set>
#include<assert.h>
#include<iostream>
typedef std::pair<char, std::string> EXPR;
class CGrammer
	//注意，0作为空使用，输入的时候必须拓扑有序
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
	bool static Merge(std::set<char>& x, std::set<char>& y, int without = -1)
	{
		bool ishave = false;
		for (auto w : y)
			if (w != without)
				x.insert(w);
			else ishave = true;
		return ishave;
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
	std::vector<std::string> &getExprs(char u) {
		return exprs[u];
	}
	std::set<char>& getVns() {
		return Vn;
	}
	std::set<char>& getVts() {
		return Vt;
	}
	std::set<char> Get_First(std::string s)
	{
		std::set<char>fir; int i = 0;
		for (; i < s.size(); ++i)
		{
			if (in_Vn(s[i])) {
				bool can_empty = Merge(fir, Fir[s[i]], '0');
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
	void Get_Follow(char x, std::string s)
	{
		if (x == 'S') Fol[x].insert('$');
		for (int i = 0; i < s.size(); ++i)
		{
			if (in_Vn(s[i]))
			{
				bool can_empty = Merge(Fol[s[i]], Get_First(s.substr(i + 1, -1)), '0');
				if (can_empty)
					Merge(Fol[s[i]], Fol[x]);
			}
		}
	}
	void initialize()
	{
		for (int x = Rules.size() - 1; x >= 0; --x)
			Merge(Fir[Rules[x].first], Get_First(Rules[x].second));
		for (int x = 0; x < Rules.size(); ++x)
			Get_Follow(Rules[x].first, Rules[x].second);
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

