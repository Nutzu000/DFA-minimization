#include "AFD.h"

AFD::AFD() :
	m_sigma{ "" },
	m_stareInitiala{ "" }
{
}

std::vector<std::string> AFD::getStari() const
{
	return m_stari;
}

std::string AFD::getSigma() const
{
	return m_sigma;
}

std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> AFD::getDelta() const
{
	return m_delta;
}

std::string AFD::getStareInitiala() const
{
	return m_stareInitiala;
}

std::vector<std::string> AFD::getFinale() const
{
	return m_finale;
}

void AFD::setStari(std::vector<std::string> q)
{
	for (auto it : q) {
		m_stari.push_back(it);
	}
}

void AFD::setSigma(const std::string& sigma) {
	m_sigma = sigma;
}

void AFD::setDelta(std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> delta)
{
	m_delta = delta;
}

void AFD::setStareInitiala(const std::string& q0)
{
	m_stareInitiala = q0;
}

void AFD::setFinale(std::vector<std::string> f)
{
	for (auto it : f) {
		m_finale.push_back(it);
	}
}

void AFD::citireFisier(const std::string& fisier)
{
	std::ifstream f(fisier);
	std::string aux;
	std::getline(f, aux);
	std::istringstream iss(aux);
	while (iss >> aux) {
		m_stari.push_back(aux);
	}
	std::getline(f, m_sigma);
	while (m_sigma.find(" ") != std::string::npos) {
		m_sigma.replace(m_sigma.find(" "), 1, "");
	}
	std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> auxDelta;
	for (int i = 0; i < m_stari.size() * m_sigma.length(); i++) {
		std::getline(f, aux);
		std::istringstream iss(aux);
		std::string starePlecare, simbol, stareFinala;
		iss >> starePlecare >> simbol >> stareFinala;
		auxDelta[std::make_pair(starePlecare, simbol)] = stareFinala;
	}
	this->setDelta(auxDelta);
	f >> m_stareInitiala;
	std::getline(f, aux);
	std::getline(f, aux);
	std::istringstream iss2(aux);
	while (iss2 >> aux) {
		m_finale.push_back(aux);
	}

}

std::string AFD::accepta(const std::string& cuvant)
{
	std::string stareaCurenta = m_stareInitiala;
	for (auto& caracter : cuvant) {
		std::string simbol(1, caracter);
		stareaCurenta = m_delta[std::make_pair(stareaCurenta, simbol)];
		if (stareaCurenta == "vid") {
			return "BLOCAT";
		}
	}
	int esteFinala = 0;
	for (auto& it : m_finale) {
		if (it.find(stareaCurenta) != std::string::npos) {
			return "ACCEPTAT";
		}
	}
	return "NEACCEPTAT";
}

int AFD::verifica()
{
	int existaStareaInitiala = 0;
	for (int i = 0; i < m_stari.size() && !existaStareaInitiala; i++) {
		if (m_stari[i].find(m_stareInitiala) != std::string::npos) {
			existaStareaInitiala = 1;
		}
	}
	int existaUnFinal = 1, existaFinale = 1;
	for (int i = 0; i < m_finale.size() && existaUnFinal; i++) {
		existaFinale = 0;
		for (int j = 0; j < m_stari.size() && existaUnFinal; j++) {
			if (m_finale[i].find(m_stari[j]) != std::string::npos) {
				existaFinale = 1;
			}
		}
		if (existaFinale == 0) {
			existaUnFinal = 0;
		}
	}
	for (auto& it : m_delta) {
		int existaStare = 0;
		for (auto& stari : m_stari) {
			if (stari == it.first.first) {
				existaStare = 1;
			}
		}
		int existaSimbol = 0;
		if (m_sigma.find(it.first.second) != std::string::npos) {
			existaSimbol = 1;
		}

		int existaStari2 = 0;
		for (auto& stari : m_stari) {
			if (stari == it.second || "vid" == it.second) {
				existaStari2 = 1;
			}
		}
		if (!existaStare || !existaSimbol || !existaStari2) {
			return 0;
		}
	}
	return existaStareaInitiala && existaFinale;
}

void AFD::stergeNeaccesibile()
{
	std::vector<int> esteVizitat;
	for (const auto& stari : m_stari) {
		esteVizitat.push_back(0);
	}

	std::queue<std::string> bfs;

	bfs.push(m_stareInitiala);
	esteVizitat[pozitieInStari(m_stareInitiala)] = 1;

	while (!bfs.empty()) {
		for (const auto& caracter : m_sigma) {
			std::string caracterString;
			caracterString.push_back(caracter);
			if (m_delta[std::make_pair(bfs.front(), caracterString)] != "vid") {
				if (!esteVizitat[pozitieInStari(m_delta[std::make_pair(bfs.front(), caracterString)])]) {
					bfs.push(m_delta[std::make_pair(bfs.front(), caracterString)]);
					esteVizitat[pozitieInStari(m_delta[std::make_pair(bfs.front(), caracterString)])] = 1;
				}
			}
		}
		bfs.pop();
	}
	for (int i = 0; i < m_stari.size(); i++) {
		if (!esteVizitat[i]) {
			esteVizitat.erase(esteVizitat.begin() + i);
			std::cout << std::endl;
			std::string stareDeSters = m_stari[i];
			std::vector<std::string>::iterator deSters = m_stari.begin() + i;
			m_stari.erase(deSters);
			if (find(m_finale.begin(), m_finale.end(), stareDeSters) != m_finale.end()) {
				m_finale.erase(find(m_finale.begin(), m_finale.end(), stareDeSters));
			}
			for (const auto& caracter : m_sigma) {
				std::string caracterString;
				caracterString.push_back(caracter);
				m_delta.erase(m_delta.find(std::make_pair(stareDeSters, caracterString)));
			}
			i--;
		}
	}
}

int AFD::pozitieInStari(const std::string& stare)
{
	return find(m_stari.begin(), m_stari.end(), stare) - m_stari.begin();
}

void AFD::minimizare()
{
	stergeNeaccesibile();
	std::unordered_map<std::pair<std::string, std::string>, int, pair_hash> perechi;
	for (int i = 0; i < m_stari.size() - 1; i++) {
		for (int j = i + 1; j < m_stari.size(); j++) {
			if (find(m_finale.begin(), m_finale.end(), m_stari[i]) != m_finale.end() ^ find(m_finale.begin(), m_finale.end(), m_stari[j]) != m_finale.end()) {
				perechi[std::make_pair(m_stari[i], m_stari[j])] = 1;
			}
			else {
				perechi[std::make_pair(m_stari[i], m_stari[j])] = -1;
			}
		}
	}
	/*for (const auto& it : perechi) {
		if (it.second == -1) {
			std::cout << it.first.first << ' ' << it.first.second << ' ' << it.second << std::endl;
		}
	}*/
	int modificare = 1;
	while (modificare) {
		modificare = 0;
		for (auto& pereche : perechi) {
			if (pereche.second == -1) {
				for (const auto& caracter : m_sigma) {
					std::string caracterString;
					caracterString.push_back(caracter);
					if (perechi[std::make_pair(
						m_delta[std::make_pair(pereche.first.first, caracterString)],
						m_delta[std::make_pair(pereche.first.second, caracterString)]
					)] == 1 || perechi[std::make_pair(
						m_delta[std::make_pair(pereche.first.second, caracterString)],
						m_delta[std::make_pair(pereche.first.first, caracterString)]
					)] == 1) {
						pereche.second = 1;
						modificare = 1;
					}
				}
			}
		}
	}
	for (const auto& it : perechi) {
		if (it.second == -1) {
			std::cout << it.first.first << ' ' << it.first.second << ' ' << it.second << std::endl;
		}
	}
	std::cout << std::endl;
	for (const auto& it : perechi) {
		if (it.second == -1) {
			if (find(m_stari.begin(), m_stari.end(), it.first.second) != m_stari.end()) {
				m_stari.erase(find(m_stari.begin(), m_stari.end(), it.first.second));
			}
			if (find(m_finale.begin(), m_finale.end(), it.first.second) != m_finale.end()) {
				m_finale.erase(find(m_finale.begin(), m_finale.end(), it.first.second));
			}
			for (const auto& caracter : m_sigma) {
				std::string caracterString;
				caracterString.push_back(caracter);
				/*if (m_delta[std::make_pair(it.first.first, caracterString)]=="vid") {
					m_delta[std::make_pair(it.first.first, caracterString)] = 
					m_delta[std::make_pair(it.first.second, caracterString)];
					std::cout<<m_delta[std::make_pair(it.first.second, caracterString)];
				}*/
				if (m_delta.find(std::make_pair(it.first.second, caracterString)) != m_delta.end()) {
					m_delta.erase(m_delta.find(std::make_pair(it.first.second, caracterString)));
				}
			}
			for (auto& tranzitie : m_delta) {
				if (tranzitie.second == it.first.second) {
					tranzitie.second = it.first.first;
				}
			}
		}
	}
}

void AFD::afisareInFisier(const std::string& fisier)
{
	std::ofstream o(fisier);
	for (auto it : m_stari) {
		o << it << ' ';
	}
	o << std::endl;
	for (auto it : m_sigma) {
		o << it << ' ';
	}
	o << std::endl;
	for (auto it : m_delta) {
		o << it.first.first << ' ' << it.first.second << ' ' << it.second << ' ';
		o << std::endl;
	}
	o << m_stareInitiala << std::endl;
	for (auto it : m_finale) {
		o << it << ' ';
	}
}


std::ostream& operator<<(std::ostream& f, const AFD& afd)
{
	f << "AFD={\n";

	for (auto it : afd.getStari()) {
		f << it << ' ';
	}
	f << "\nSigma:";
	for (auto it : afd.getSigma()) {
		f << it << ' ';
	}
	f << "\nDelta:\n";
	for (auto it : afd.getDelta()) {
		f << it.first.first << ' ' << it.first.second << ' ' << it.second << ' ';
		f << std::endl;
	}
	f << "Starea initiala: " << afd.getStareInitiala()
		<< "\nStarile finale: ";
	for (auto it : afd.getFinale()) {
		f << it << ' ';
	}
	f << "\n}";
	return f;
}

