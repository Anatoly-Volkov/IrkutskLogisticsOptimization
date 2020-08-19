#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <string>
#include <ctime>
#include <fstream>
#include <string>
#include <map>
#include <time.h>

using namespace std;

int exitcode = 0;
const char ch = '"';
int fordebug = 0;
int n, m;
int filekey;
vector<vector<int> > time_matrix;

struct order {
	double x; // location
	double y; // location
	int a; // time start
	int b; // time end
	int z; // lasts z hours
	int id; // number of the nearest point
	order() {}
};

struct worker {
	double x;  // location
	double y;  // location
	int a;  // time start
	int b;  // time end
	int S;  //salary for working per hour
	int S1 = 0; //salary for moving per hour
	int S2 = 0; //salary for doing nothing per hour
	int id; // number of the nearest point
	worker() {}
};

worker input_worker() {
	int a, b, S, S1, S2;
	double x, y;
	cin >> x >> y;
	cin >> a >> b;
	cin >> S >> S1 >> S2;
	worker W;
	W.x = x; W.y = y; W.a = a; W.b = b; W.S = S; W.S1 = S1; W.S2 = S2;
	return W;
}

order input_order() {
	int a, b, z;
	double x, y;
	cin >> x >> y;
	cin >> a >> b;
	cin >> z;
	order O;
	O.x = x; O.y = y; O.a = a; O.b = b; O.z = z;
	return O;
}

double strtodouble(string s) {
	int big = 0;
	double small = 0;
	int cnt_sm = 0;
	int minus = 1;
	int flag = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '.') {
			flag = 1;
			continue;
		}
		if (s[i] == '-') {
			minus = -1;
			continue;
		}
		if (flag == 0) {
			big *= 10;
			big += (s[i] - '0');
		}
		if (flag == 1) {
			small *= 10;
			small += (s[i] - '0');
			cnt_sm++;
		}
	}
	for (int i = 0; i < cnt_sm; i++) small /= 10;
	small += big;
	return small * minus;
}

int strtoint(string s) {
	int now = 0;
	int flag = 1;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '-') flag = -1;
		else {
			now *= 10;
			now += s[i] - '0';
		}
	}
	return now * flag;
}

string inttostr(int x) {
	string now = "";
	int digit;
	char symb;
	while (x > 0) {
		digit = x % 10;
		x /= 10;
		symb = '0' + digit;
		now = symb + now;
	}
	return now;
}

string input_json() {
	string json = "";
	string help_input;
	int bal = 0;
	while (cin >> help_input) {
		for (auto elem : help_input) {
			if (elem == '{') bal++;
			if (elem == '}') bal--;
			json += elem;
		}
		if (bal == 0) break;
	}
	return json;
}

map<string, string> json_parser(string json) {
	json += "     ";
	//states:
	// 0 - search name_of_type   "NAME"
	// 1 - adding new name_of_type
	// 2 - searching for value
	// 3 - adding new value (in "....")
	//3 - adding new value (in [.....])
	map<string, string> type_name;
	string name = "";
	string value = "";
	int state = 0;
	for (int i = 0; i < json.size() - 5; i++) {
		if (state == 0) {
			if (json[i] == '"') {
				state = 1;
				continue;
			}
		}
		if (state == 1) {
			if (json[i] == '"') {
				state = 2;
				continue;
			}
			else {
				name += json[i];
			}
		}
		if (state == 2) {
			if (json[i] == '"') {
				state = 3;
				continue;
			}
			if (json[i] == '[') {
				state = 4;
				continue;
			}
		}
		if (state == 3) {
			if (json[i] == '"') {
				state = 0;
				type_name[name] = value;
				name = "";
				value = "";
				continue;
			}
			value += json[i];
		}
		if (state == 4) {
			if (json[i] == ']') {
				state = 0;
				type_name[name] = value;
				name = "";
				value = "";
				continue;
			}
			value += json[i];
		}
	}
	return type_name;
}

void json_analize(string json, vector<worker> &man, vector<order> &all) {
	map<string, string> type_name = json_parser(json);
	int state = 0;
	string help = "";
	string arr;
	map<string, string> person;
	if (type_name["RTYPE"] == "0") {
		filekey = rand() % 1000000;
		cout << "{" << ch << "ATYPE" << ch << " : " << ch << "0" << ch << ", " << ch << "KEY" << ch << " : " << ch << inttostr(filekey) << ch << "}" << endl;
		n = strtoint(type_name["n"]);
		m = strtoint(type_name["m"]);
		arr = type_name["workers"];
		//state
		// 0 - before
		// 1 - in
		int state = 0;
		for (auto elem : arr) {
			if (state == 0) {
				if (elem == '{') {
					state = 1;
					continue;
				}
			}
			if (state == 1) {
				if (elem == '}') {
					state = 0;
					person = json_parser(help);
					help = "";
					worker A;
					A.x = strtodouble(person["x"]);
					A.y = strtodouble(person["y"]);
					A.a = strtoint(person["start"]);
					A.b = strtoint(person["end"]);
					A.S = strtoint(person["price"]);
					man.push_back(A);
					continue;
				}
				help += elem;
			}
		}


		arr = type_name["orders"];
		//state
		// 0 - before
		// 1 - in
		state = 0;
		for (auto elem : arr) {
			if (state == 0) {
				if (elem == '{') {
					state = 1;
					continue;
				}
			}
			if (state == 1) {
				if (elem == '}') {
					state = 0;
					person = json_parser(help);
					help = "";
					order A;
					A.x = strtodouble(person["x"]);
					A.y = strtodouble(person["y"]);
					A.a = strtoint(person["start"]);
					A.b = strtoint(person["end"]);
					A.z = strtoint(person["duration"]);
					all.push_back(A);
					continue;
				}
				help += elem;
			}
		}
	}
	else {
		string filename = type_name["KEY"];
		filename += ".txt";
		char * filename1 = new char[filename.size() + 1];
		for (int ik = 0; ik < filename.size(); ik++) filename1[ik] = filename[ik];
		filename1[filename.size()] = '\0';
		freopen(filename1, "r", stdin);
		json = input_json();
		cout << json << endl;
		exitcode = 1;
	}
}

string time_format(double x) {
	if (x - (int)x < 0.001) return ":00";
	return ":30";
}

double geom_dist(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double dist_time(int id1, int id2) {
	int value = time_matrix[id1][id2];
	double d = (int)(value / 60);
	value %= 60;
	if (value < 5) return d;
	if (value < 35) return d + 0.5;
	return d + 1;
}

int cnt(worker A, vector<order> &need, vector<int> &arr) {
	//arr is the permutation of numbers from 0 to n where n - number of orders which shows the right order of ordrers in need
	//returns salary of worker
	order elem;
	int lastid = A.id;
	double last_time = A.a;
	double next_time, transport;
	double help;
	int n = need.size();
	int value = 0;
	for (int i = 0; i < n; i++) {
		elem = need[arr[i]];
		transport = dist_time(lastid, elem.id);
		next_time = max(last_time + transport, (double)elem.a) + elem.z;
		//if (fordebug == 1) cout << last_time << " " << next_time << " " << elem.z << " " << elem.b << " " << A.b << endl;
		if (next_time > elem.b || next_time > A.b) {
			value += 1000000 * (n - i);
			return value;
		}
		value += A.S * elem.z;
		value += (int)(A.S1 * transport);
		value += (int)(A.S2 * max((double)0, elem.a - transport - last_time));
		lastid = elem.id;
		last_time = next_time;
	}
	value += (int)(A.S2 * (A.b - last_time));
	return value;
}

void gen(vector<vector<int> > &all, vector<int> &now, vector<int> &used, int deep, int n) {
	//generates permutation
	if (deep == n) {
		all.push_back(now);
		return;
	}
	for (int i = 0; i < n; i++) {
		if (used[i] == 0) {
			used[i] = 1;
			now.push_back(i);
			gen(all, now, used, deep + 1, n);
			now.pop_back();
			used[i] = 0;
		}
	}
}

pair<vector<int>, int> distribute(worker A, vector<order> &need) {
	//distributes orderes for one worker and chooses the best
	int n = need.size();
	vector<vector<int> > all;
	vector<int> used(n);
	vector<int> now;
	for (int i = 0; i < n; i++) used[i] = 0;
	gen(all, now, used, 0, n);
	now.clear();
	int cost = 1e9;
	int cost1;
	for (auto elem : all) {
		cost1 = cnt(A, need, elem);
		if (cost1 < cost) {
			cost = cost1;
			now = elem;
		}
	}
	return{ now, cost };
}

long double random_double(long double a) {
	//double random from 0 to a
	return ((long double)rand() / RAND_MAX) * a;
}

int main() {
	srand(time(NULL));
	//--------------input---------------------------
	//int n, m; //n - workers, m - orders
	vector<worker> man;
	vector<order> all;
	//FILE* inp = freopen("input.txt", "r", stdin);
	//FILE* outp = freopen("output.txt", "w", stdout);
	string json = input_json();
	json_analize(json, man, all);
	//fclose(inp);
	//fclose(outp);
	if (exitcode == 1) return 0;
	FILE* inp = freopen("Points.txt", "r", stdin);
	int n1;
	double latitude, longitude;
	cin >> n1;
	vector<pair<double, double> > origins(n1);
	for (int i = 0; i < n1; i++) {
		cin >> latitude >> longitude;
		origins[i] = { latitude, longitude };
	}
	int time_elem;
	for (int i = 0; i < n1; i++) {
		time_matrix.push_back(vector<int>());
		for (int j = 0; j < n1; j++) {
			cin >> time_elem;
			time_matrix[i].push_back(time_elem);
		}
	}
	fclose(inp);
	int id_now = 0;
	double dist_now = 0;
	double ans_dist = 100000000;

	for (int i = 0; i < n; i++) {
		id_now = 0;
		ans_dist = 100000000;
		for (int j = 0; j < n1; j++) {
			dist_now = geom_dist(man[i].x, man[i].y, origins[j].first, origins[j].second);
			if (dist_now < ans_dist) {
				ans_dist = dist_now;
				id_now = j;
			}
		}
		man[i].id = id_now;
	}

	for (int i = 0; i < m; i++) {
		id_now = 0;
		ans_dist = 100000000;
		for (int j = 0; j < n1; j++) {
			dist_now = geom_dist(all[i].x, all[i].y, origins[j].first, origins[j].second);
			if (dist_now < ans_dist) {
				ans_dist = dist_now;
				id_now = j;
			}
		}
		all[i].id = id_now;
	}

	//--------------genetica-----------------------------

	int x;
	int k = 30; //number of creatures
	int mut_level = k / 10;
	vector<vector<int> > answer;
	int ans = 1e9;
	vector<order> need;
	vector<vector<vector<int> > > animal(k); //array of creatures animal[k][n][...]
											 //animal[i][j] - array of orders of j worker in i creature
	vector<int> mother(m);
	vector<int> father(m);
	vector<int> now_cnt(n);
	vector<vector<vector<int> > > create(k);
	vector<int> price(k);
	vector<long double> cool(k);

	//creating creatures
	for (int ij = 0; ij < k; ij++) {
		for (int i = 0; i < n; i++) animal[ij].push_back(vector<int>());
		for (int i = 0; i < m; i++) {
			x = rand() % n;
			while (animal[ij][x].size() > 5) x = rand() % n;
			animal[ij][x].push_back(i);
		}
	}
	int mutation_const;
	int minim = 1e9;
	int last_minim = 1e9;
	int min_ind = 0;
	for (int ijk = 0; ijk < 500; ijk++) {
		//cerr << ijk << ": ";
		time_t last_speed_test = time(NULL);
		minim = 1e9;

		//calculating prices
		for (int i = 0; i < k; i++) {
			price[i] = 0;
			for (int j = 0; j < n; j++) {
				need.clear();
				for (auto elem : animal[i][j]) need.push_back(all[elem]);
				auto res = distribute(man[j], need);
				price[i] += res.second;
			}
			if (minim > price[i]) {
				minim = price[i];
				min_ind = i;
			}
			if (minim < ans) {
				answer = animal[i];
				ans = minim;
			}
			cool[i] = price[i];
			cool[i] /= 1000;
			cool[i] = 1 / cool[i];
			if (i != 0) cool[i] += cool[i - 1];
		}


		//cerr << minim << endl;


		//checking progress of generation
		if (minim >= last_minim) mut_level++;
		else mut_level--;

		mut_level = min(mut_level, k / 2);
		mut_level = max(1, mut_level);
		last_minim = minim;

		//----------------new generation-------------------------
		for (int ij = 0; ij < k; ij++) {
			int c1, c2;
			long double point;

			//choosing father and mother

			point = random_double(cool[k - 1]);
			for (int i = 0; i < n; i++) now_cnt[i] = 0;
			c1 = 0;
			while (c1 < k && cool[c1] < point) c1++;
			c1 = min(c1, k - 1);
			for (int i = 0; i < n; i++) {
				for (auto elem : animal[c1][i]) {
					mother[elem] = i;
				}
			}

			point = random_double(cool[k - 1]);
			c2 = 0;
			while (c2 < k && cool[c2] < point) c2++;
			c2 = min(c2, k - 1);
			for (int i = 0; i < n; i++) {
				for (auto elem : animal[c2][i]) {
					father[elem] = i;
				}
			}

			//creating a child
			for (int i = 0; i < m; i++) {
				if (now_cnt[mother[i]] > 5) {
					mother[i] = father[i];
					now_cnt[father[i]]++;
					continue;
				}
				if (now_cnt[father[i]] > 5) {
					now_cnt[mother[i]]++;
					continue;
				}
				x = rand() % 2;
				if (x == 1) mother[i] = father[i];
				now_cnt[mother[i]]++;
			}

			create[ij].clear();
			for (int i = 0; i < n; i++) {
				create[ij].push_back(vector<int>());
			}

			//mutation
			mutation_const = rand() % 30;
			if (mutation_const < mut_level) {
				mutation_const = rand() % m;
				mother[mutation_const] = rand() % n;
			}

			for (int i = 0; i < m; i++) {
				create[ij][mother[i]].push_back(i);
			}
		}
		animal = create;

		time_t now_speed_test = time(NULL);
	}

	//getting answer

	int pr;
	for (int i = 0; i < k; i++) {
		pr = 0;
		for (int j = 0; j < n; j++) {
			need.clear();
			for (auto elem : animal[i][j]) need.push_back(all[elem]);
			auto res = distribute(man[j], need);
			pr += res.second;
		}
		if (pr < ans) {
			ans = pr;
			answer = animal[i];
		}
	}
	string filename = inttostr(filekey);
	filename += ".txt";
	char * filename1 = new char[filename.size() + 1];
	for (int ik = 0; ik < filename.size(); ik++) filename1[ik] = filename[ik];
	//copy(filename.begin(), filename.end(), filename1);
	filename1[filename.size()] = '\0';
	//cout << filename << " " << filename1 << endl;
	FILE* file = fopen(filename1, "w");
	fclose(file);
	//
	freopen(filename1, "w", stdout);
	//1 million is one uncomplited order
	cout << "{" << ch << "ATYPE" << ch << " : " << ch << "1" << ch << ", ";
	cout << ch << "POSSIBILITY" << ch << " : ";
	if (ans >= 1000000) cout << ch << "0" << ch << ", ";
	else cout << ch << "1" << ch << ", ";
	cout << ch << "PRICE" << ch << " : " << ch << ans << ch << ", ";
	cout << ch << "n" << ch << " : " << ch << n << ch << ", ";
	cout << ch << "workers" << ch << " : ";
	cout << "[ ";
	//if (ans >= 1000000) cerr << "Unfortunatelly you can't distribute your orders" << endl;
	//cerr << "Price: " << ans << endl;
	int nowid;
	double nowtime, for_move;
	string forprint = "";
	for (int i = 0; i < n; i++) {
		need.clear();
		cout << "{ " << ch << "id" << ch << " : " << ch << i + 1 << ch << ", ";
		cout << ch << "m" << ch << " : " << ch << answer[i].size() << ch << ", ";
		for (auto elem : answer[i]) need.push_back(all[elem]);
		auto res = distribute(man[i], need);
		cout << ch << "PRICE" << ch << " : " << ch << res.second << ch << ", ";
		//cerr << "Worker " << i + 1 << ": " << endl;
		cout << ch << "orders" << ch << " : ";
		cout << "[ ";
		nowid = man[i].id;
		//nowx = man[i].x; nowy = man[i].y;
		nowtime = man[i].a;
		for (int ik = 0; ik < res.first.size(); ik++) {
			auto elem = res.first[ik];
			for_move = dist_time(nowid, all[answer[i][elem]].id);
			nowtime = max(nowtime + for_move, (double)all[answer[i][elem]].a);
			forprint = time_format(nowtime);
			cout << "{ " << ch << "id" << ch << " : " << ch << answer[i][elem] + 1 << ch << ", ";
			cout << ch << "start" << ch << " : " << ch << (int)nowtime << forprint << ch << ", ";
			cout << ch << "end" << ch << " : " << ch << (int)nowtime + all[answer[i][elem]].z << forprint << ch << " }";
			if (ik != res.first.size() - 1) cout << ", ";
			//cerr << " Order#" << answer[i][elem] + 1 << ": " << (int)nowtime << forprint << " - " << (int)nowtime + all[answer[i][elem]].z << forprint << endl;
			nowtime += all[answer[i][elem]].z;
			nowid = all[answer[i][elem]].id;
		}
		cout << "] }";
		if (i != n - 1) cout << ", ";
		//cerr << " " << res.second << endl;
	}
	cout << "] ";
	cout << "}" << endl;
}
