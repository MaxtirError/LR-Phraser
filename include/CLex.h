#pragma once
#include<string>
#include<vector>
#include<stack>
#include<set>
#include<map>
#include<queue>
typedef std::set<int> Status;
class CLex
{

};
struct XFA_Edges {
	std::map<char, int> Edges;
	std::vector<int>Zero_link;
	void add(int v, char w) {
		if (w == '0')
			Zero_link.push_back(v);
		else Edges[w] = v;
	}
	int next(char w) {
		if (Edges.find(w) != Edges.end())
			return Edges[w];
		else return -1;
	}
	void clear() {
		Edges.clear();
		Zero_link.clear();
	}
};
class XFA {
public:
	XFA() { Vn = 0; };
	XFA(char c) {
		Vn = 2;
		s = 0; f = 1;
		Vert_edges.push_back(XFA_Edges());
		Vert_edges.push_back(XFA_Edges());
		add(0, 1, c);
	}
	void add(int u, int v, char w) {
		Vert_edges[u].add(v, w);
		Sig.insert(w);
	}
	void rep() {
		for (int i = 0; i < Vn; ++i) {
			for (auto& e : Vert_edges[i].Edges)
				e.second++;
			for (auto& x : Vert_edges[i].Zero_link)
				++x;
		}
		Vert_edges.push_back(XFA_Edges());
		for (int i = Vn; i >= 1; --i)
			Vert_edges[i] = Vert_edges[i - 1];
		Vert_edges[0].clear();
		add(s, 1, '0');
		add(f, 1, '0');
		add(f, f + 1, '0');
		add(s, f + 1, '0');
		++f;
	}
	static friend::XFA Link(std::vector<XFA> S) {
		XFA A = S[0];
		for (int i = 1; i < S.size(); ++i) {
			for (int j = 0; j < S[i].Vn; ++j) {
				XFA_Edges e = S[i].Vert_edges[j];
				for (auto& x : e.Edges) {
					if (x.second == 0)
						x.second = A.f;
					else x.second += A.Vn - 1;
					if (j == 0)
						A.Vert_edges[A.f].add(x.second, x.first);
				}
				for (auto& x : e.Zero_link) {
					if (x == 0)
						x = A.f;
					else x += A.Vn - 1;
					if (j == 0)
						A.Vert_edges[A.f].add(x, '0');
				}
				if (j)
					A.Vert_edges.push_back(e);
			}
			A.Vn += S[i].Vn - 1;
			A.f = A.Vn - 1;
			for (auto x : S[i].Sig)
				A.Sig.insert(x);
		}
		return A;
	}
	static friend::XFA Merge(std::vector<XFA> S) {
		if (S.size() == 1)
			return S[0];
		XFA A;
		A.Vn = 1; A.s = 0; A.f = 1;
		A.Vert_edges.push_back(XFA_Edges());
		for (auto& B : S)
			A.f += B.Vn;
		for (auto &B : S) {
			A.add(A.s, A.Vn + B.s, '0');
			for (auto e : B.Vert_edges) {
				for (auto& x : e.Edges)
					x.second += A.Vn;
				for (auto& x : e.Zero_link)
					x += A.Vn;
				A.Vert_edges.push_back(e);
			}
			A.add(B.f + A.Vn, A.f, '0');
			A.Vn += B.Vn;
			for (auto x : B.Sig)
				A.Sig.insert(x);
		}
		A.Vert_edges.push_back(XFA_Edges());
		++A.Vn;
		return A;
	}
	void printinfo() {
		if (f != -1) {
			printf("An NFA with s:%d f:%d and edges:", s, f);
		}
		else {
			printf("An DFA with s:%d ", s);
			printf("f:{");
			for (auto x : F)
				printf("%d ", x);
			printf("}and edges:");
		}
		for (int v = 0; v < Vert_edges.size(); ++v) {
			for (auto x : Vert_edges[v].Edges)
				printf("(%d,%d,%c)", v, x.second, x.first);
			for (auto x : Vert_edges[v].Zero_link)
				printf("(%d,%d,%c)", v, x, '0');
		}
		puts("");
	}
	Status GoTo(Status Verts, char c) {
		Status nx; int nx_v;
		for (auto v : Verts)
			if((nx_v = Vert_edges[v].next(c)) != -1)
				nx.insert(nx_v);
		return nx;
	}
	Status get_closure(Status S) {
		std::queue<int>q;
		for (auto s : S)
			q.push(s);
		for (; !q.empty();) {
			int u = q.front(); q.pop();
			for(auto v : Vert_edges[u].Zero_link) 
				if (S.insert(v).second)
					q.push(v);
			}
		return S;
	}
	void cast_to_DFA() {
		XFA A; A.s = 0; A.f = -1;
		std::map<Status, int> mp;
		Status s0; s0.insert(0);
		s0 = get_closure(s0);
		int id = 0; mp[s0] = 0;
		std::queue<Status > q;
		q.push(s0);
		for (; !q.empty();) {
			const Status S = q.front(); q.pop();
			printf("cuurentS:");
			for (auto x : S)
				printf("%d ", x);
			puts("");
			XFA_Edges e;
			for (auto c : Sig) 
			if(c != '0') {

				Status nx = get_closure(GoTo(S, c));
				printf("%c to nxS:", c);

				for (auto x : nx)
					printf("%d ", x);
				puts("");
				
				/*printf("%c goto nxS:", c);
				for (auto x : GoTo(S, c))
					printf("%d ", x);
				puts("");*/
				if (!nx.empty()) {
					int nx_id;
					if (mp.find(nx) != mp.end())
						nx_id = mp[nx];
					else {
						nx_id = mp[nx] = ++id;
						q.push(nx);
					}
					e.add(nx_id, c);
				}
			}
			A.Vert_edges.push_back(e);
			if (S.find(f) != S.end())
				A.F.insert(A.Vn);
			A.Vn++;
		}
		Sig.erase('0');
		A.Sig = Sig;
		*this = A;
	}
	bool isfull() {
		for (int i = 0; i < Vn; ++i)
			if (Vert_edges[i].Edges.size() != Sig.size())
				return false;
		return true;
	}
	void Minimize() {
		bool haveded = false;
		if (!isfull()) {
			haveded = true;
			for (int i = 0; i < Vn; ++i) {
				for (char c : Sig)
					if (Vert_edges[i].next(c) == -1)
						Vert_edges[i].add(Vn, c);
			}
			Vert_edges.push_back(XFA_Edges());
			for (char c : Sig)
				Vert_edges[Vn].add(Vn, c);
			Vn++;
		}
		int* Class_id = new int[Vn];
		std::set<Status>Classes;
		for (int i = 0; i < Vn; ++i)
			Class_id[i] = 0;
		for (auto f : F)
			Class_id[f] = 1;
		Status c0, c1;
		for (int i = 0; i < Vn; ++i)
			if (Class_id[i] == 0)
				c0.insert(i);
			else c1.insert(i);
		Classes.insert(c0);
		Classes.insert(c1);
		int* to = new int[Vn];
		int num_id = 2;
		for (;;) {
			/*for (int i = 0; i < Vn; ++i)
				printf("%d ", Class_id[i]);
			puts("");*/
			bool split = false;
			for(auto C : Classes)
				if (C.size() >= 2) 
				{
					for (char ch : Sig) 
					{
						std::map<int, std::set<int> >mp;
						for (int s : C)
							mp[Class_id[Vert_edges[s].Edges[ch]]].insert(s);
						if (mp.size() > 1) { //split
							split = true;
							Classes.erase(C);
							bool first = true;
							for (auto x : mp) {
								Classes.insert(x.second);
								if (first)
									first = false;
								else {
									for (auto s : x.second)
										Class_id[s] = num_id;
									++num_id;
								}
							}
							break;
						}
					}
					if (split)
						break;
				}
			if (!split)
				break;
		}
		XFA A;
		if (haveded) {
			int* nClass_id = new int[Vn];
			for (int i = 0; i < Vn; ++i)
				nClass_id[i] = -1;
			num_id = 0;
			for (auto C : Classes) {
				bool die = false;
				for (auto x : C)
					if (Class_id[x] == Class_id[Vn - 1]) {
						die = true;
						break;
					}
					else nClass_id[x] = num_id;
				num_id += !die;
			}
			for (int i = 0; i < Vn; ++i)
				Class_id[i] = nClass_id[i];
			/*for (int i = 0; i < Vn; ++i)
				printf("%d ", Class_id[i]);
			puts("");*/
			delete[] nClass_id;
		}
		A.Vn = num_id;
		A.s = Class_id[s];
		A.f = -1;
		A.Vert_edges.resize(num_id);
		for (auto f : F)
			A.F.insert(Class_id[f]);
		for (int i = 0; i < Vn; ++i)
			if (Class_id[i] != -1)
				for (auto e : Vert_edges[i].Edges)
					if(Class_id[e.second] != -1)
						A.add(Class_id[i], Class_id[e.second], e.first);
		*this = A;
	}
private:
	int Vn, s, f;
	std::vector<XFA_Edges> Vert_edges;
	std::set<char>Sig;
	std::set<int>F;
};

class Regular
{
private:
	std::string raw_string;
public:
	Regular(std::string _string = "") { raw_string = _string; }
	~Regular() {}
	XFA cast_to_XFA() {
		std::stack<int>st;
		int* mat = new int[raw_string.size()]{0};
		for (int i = 0; i < raw_string.size(); ++i) 
		if(raw_string[i] == '(')
			st.push(i);
		else if (raw_string[i] == ')') {
			mat[st.top()] = i;
			st.pop();
		}
		std::vector<XFA>cS, oS;
		for (int i = 0; i < raw_string.size(); ++i)
		{
			switch (raw_string[i])
			{
			case '(':
				cS.push_back(Regular(raw_string.substr(i + 1, mat[i] - i - 1)).cast_to_XFA());
				i = mat[i];
				break;
			case '*':
				cS.rbegin()->rep();
				break;
			case '|':
				oS.push_back(Link(cS));
				cS.clear();
				break;
			default:
				cS.push_back(XFA(raw_string[i]));
				break;
			}
		}
		if (!cS.empty())
			oS.push_back(Link(cS));
		delete[] mat;
		return Merge(oS);
	}
};