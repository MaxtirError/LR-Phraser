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

void CPraser::SLR_Prase()
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

void CPraser::ShowInfo()
{
	puts("output praser info----------");
	printf("items size:%d\n", items.size());
	for (auto x : items) {
		x.Showinfo();
	}
}