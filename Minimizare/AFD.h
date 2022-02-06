#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <fstream>
struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};
class AFD {
private:
	std::vector<std::string> m_stari;
	std::string m_sigma;
	std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> m_delta;
	std::string m_stareInitiala;
	std::vector<std::string> m_finale;
public:
	AFD();
	std::vector<std::string> getStari()const;
	std::string getSigma() const;
	std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> getDelta() const;
	std::string getStareInitiala() const;
	std::vector<std::string> getFinale()const;
	void setStari(std::vector<std::string> q);
	void setSigma(const std::string& sigma);
	void setDelta(std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> delta);
	void setStareInitiala(const std::string& q0);
	void setFinale(std::vector<std::string> f);
	void citireFisier(const std::string& fisier);
	friend std::ostream& operator<<(std::ostream& f, const AFD& afd);
	std::string accepta(const std::string& cuvant);
	int verifica();
	void stergeNeaccesibile();
	int pozitieInStari(const std::string& stare);
	void minimizare();
	void afisareInFisier(const std::string& fisier);
};