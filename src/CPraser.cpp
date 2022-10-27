#include "CPraser.h"
void ItemSet::GetClosure()
{
	int current_size;
	do 
	{
		current_size = exprs.size();
		auto nexprs = exprs;
		for (EXPR expr : exprs)
		{
			std::string &s = expr.second;
			int pos = s.find('.');
			if (pos != s.size() - 1 && CGrammer::in_Vn(s[pos + 1]))
			{
				char vn = s[pos + 1];
				for (auto expr : grammer->getExprs(vn)) {
					std::cout << "expradding:" << vn << '.' + expr << std::endl;
					nexprs.insert(std::make_pair(vn, '.' + expr));
				}
			}
		}
		exprs = nexprs;
	} while (current_size != exprs.size());
}

ItemSet ItemSet::GoTo(char x) const
{
	ItemSet nSet(grammer);
	for (EXPR expr : exprs)
	{
		std::string s = expr.second;
		int pos = s.find('.');
		if (pos != s.size() - 1 && s[pos + 1] == x)
		{
			std::swap(s[pos], s[pos + 1]);
			nSet.add_expr(x, s);
		}
	}
	nSet.GetClosure();
	return nSet;
}

bool LL1::islegal()
{
	auto OrEmpty = [](std::set<char>& s1, std::set<char> &s2) {
		for (auto x : s2)
			if (s1.count(x))
				return false;
		return true;
	};
	for (char A : grammer.getVns())
	{
		auto exprs = grammer.getExprs(A);
		auto& A_map = Move[A];
		for (int i = 0; i < exprs.size(); ++i) 
		{
			auto Firi = grammer.Get_First(exprs[i]);
			bool isempty = false;
			for (auto x : Firi)
				if (x != '0') {
					if (A_map.count(x))
						return false;
					else A_map[x] = exprs[i];
				}
				else isempty = true;
			if (isempty) {
				for (auto x : grammer.getFol(A))
				{
					if (A_map.count(x))
						return false;
					else A_map[x] = exprs[i];
				}
			}
		}
	}
	return true;
}

void LL1::Prase() 
{
	if (!islegal()) {
		puts("this grammer is not in LL(1)");
		return;
	}
	else {
		puts("successfully prase, Moves form below");
		for (char A : grammer.getVns())
		{
			for (auto x : Move[A]) {
				printf("Move[%c, %c]=", A, x.first);
				std::cout << x.second << std::endl;
			}
		}
	}
}

void LL1::type(char A) {
	if (iserror) return;
	if (!Move[A].count(*lookahead))
		return;
	auto exprs = Move[A][*lookahead];
	for (int i = 0; i < exprs.size(); ++i)
		if (!CGrammer::in_Vn(exprs[i])) {
			match(exprs[i]);
		}
		else type(exprs[i]);

}
void LL1::match(char a) {
	if (iserror) return;
	if (a == '0')
		return;
	if (*lookahead != a)
		error();
	else lookahead++;
}

void LL1::error() {
	iserror = true;
}

bool LL1::Analyze(std::string _buffer) {
	if (isrecursion == LL1RECURSION) {
		buffer = new char[_buffer.size() + 1];
		for (int i = 0; i < _buffer.size(); ++i)
			buffer[i] = _buffer[i];
		buffer[_buffer.size()] = '$';
		lookahead = buffer;
		iserror = false;
		type('S');
		if (iserror)
			return false;
		else return *lookahead == '$';
	}
}

void SLR::Prase()
{
	ItemSet I(&grammer, 0);
	I.add_expr('Z', ".S");
	I.GetClosure();
	items.push_back(I);
	items_id[I] = 0;
	std::queue<int> q;
	q.push(0);
	do
	{
		int id = q.front(); q.pop();
		auto LinkIn = [&](ItemSet& In, char x) {
			if (In.empty()) return;
			if (items_id.find(In) != items_id.end()) {
				items[id].add_Moves(items_id[In], x);
			}
			else {
				int nid = items.size();
				In.Set_index(nid);
				items.push_back(In);
				items_id[In] = nid;
				items[id].add_Moves(nid, x);
				q.push(nid);
			}
		};
		for (auto x : grammer.getVns()) {
			ItemSet In = items[id].GoTo(x);
			LinkIn(In, x);
		}

		for (auto x : grammer.getVts()) {
			ItemSet In = items[id].GoTo(x);
			LinkIn(In, x);
		}
	} while (!q.empty());
}

void SLR::ShowInfo()
{
	puts("output praser info----------");
	printf("items size:%d\n", items.size());
	for (auto x : items) {
		x.Showinfo();
	}
}