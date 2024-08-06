// Róger Cassol e Guilherme Oscar Braun
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <map>
#include <functional>
#include <algorithm>
#include <chrono>

using namespace std;

// Mapa para converter abreviações de meses para números
map<string, string> converterMes = {
    {"Jan", "01"},
    {"Feb", "02"},
    {"Mar", "03"},
    {"Apr", "04"},
    {"May", "05"},
    {"Jun", "06"},
    {"Jul", "07"},
    {"Aug", "08"},
    {"Sep", "09"},
    {"Oct", "10"},
    {"Nov", "11"},
    {"Dec", "12"}};

// Classe para representar um registro de log
class Registro
{
private:
    string mes;
    string dia;
    string hora;
    string dataHora;
    string codigo;
    string mensagem;
    string classificacao;
    int prioridade;
    string protocolo;
    string origemIP;
    int origemPorta;
    string destinoIP;
    int destinoPorta;

public:
    Registro(string linha)
    {
        stringstream ss(linha);

        // Lendo os valores da linha do log
        getline(ss, mes, '\t');
        getline(ss, dia, '\t');
        getline(ss, hora, '\t');

        // Convertendo a data e hora para o formato desejado
        dataHora = "2024-" + converterMes[mes] + "-" + dia + " " + hora;

        getline(ss, codigo, '\t');
        getline(ss, mensagem, '\t');
        getline(ss, classificacao, '\t');
        ss >> prioridade;
        ss.ignore();
        getline(ss, protocolo, '\t');
        getline(ss, origemIP, '\t');

        // Se o valor da porta não for especificado, será representado por 0
        string origemPortaStr; // Variável Auxiliar
        getline(ss, origemPortaStr, '\t');
        if (origemPortaStr.empty())
        {
            origemPorta = 0;
        }
        else
        {
            origemPorta = stoi(origemPortaStr);
        }

        getline(ss, destinoIP, '\t');

        // Se o valor da porta não for especificado, será representado por 0
        string destinoPortaStr; // Variável Auxiliar
        getline(ss, destinoPortaStr, '\t');
        if (destinoPortaStr.empty())
        {
            destinoPorta = 0;
        }
        else
        {
            destinoPorta = stoi(destinoPortaStr);
        }
    }

    // Getters e setters para os atributos do registro
    string getMes() { return this->mes; }
    string getDia() { return this->dia; }
    string getHora() { return this->hora; }

    string getDataHora() { return this->dataHora; }
    void setDataHora(string dataHora) { this->dataHora = dataHora; }

    string getCodigo() { return this->codigo; }
    void setCodigo(string codigo) { this->codigo = codigo; }

    string getMensagem() { return this->mensagem; }
    void setMensagem(string mensagem) { this->mensagem = mensagem; }

    string getClassificacao() { return this->classificacao; }
    void setClassificacao(string classificacao) { this->classificacao = classificacao; }

    int getPrioridade() { return this->prioridade; }
    void setPrioridade(int prioridade) { this->prioridade = prioridade; }

    string getProtocolo() { return this->protocolo; }
    void setProtocolo(int protocolo) { this->protocolo = protocolo; }

    string getOrigemIP() { return this->origemIP; }
    void setOrigemIP(int origemIP) { this->origemIP = origemIP; }

    int getOrigemPorta() { return this->origemPorta; }
    void setOrigemPorta(int origemPorta) { this->origemPorta = origemPorta; }

    string getDestinoIP() { return this->destinoIP; }
    void setDestinoIP(int destinoIP) { this->destinoIP = destinoIP; }

    int getDestinoPorta() { return this->destinoPorta; }
    void setDestinoPorta(int destinoPorta) { this->destinoPorta = destinoPorta; }
};

class Sistema
{
private:
    vector<Registro *> logs;
    vector<Registro *> filtros;
    vector<function<bool(Registro *)>> funcoesDeFiltro;
    vector<string> descricaoFiltros; // Vetor de características dos Filtros(exemplo: Protocolo = ICMP)

public:
    Sistema(string nomeArquivo)
    {
        ifstream arquivo(nomeArquivo);

        if (!arquivo.is_open())
        {
            cout << "\nErro ao abrir o arquivo." << endl;
            exit(EXIT_FAILURE);
        }

        // Pulando a primeira linha (cabeçalho)
        string linha;
        getline(arquivo, linha);

        // Lendo todas as linhas em um vetor de strings
        vector<string> linhas;
        while (getline(arquivo, linha))
        {
            linhas.push_back(linha);
        }

        // Usando iterator para percorrer as linhas
        vector<string>::iterator it;
        for (it = linhas.begin(); it != linhas.end(); ++it)
        {
            logs.push_back(new Registro(*it));
        }

        arquivo.close();
    }

    // Função para analisar a data e hora e converter para o tipo time_point
    chrono::system_clock::time_point analisarData(const string &dateTimeStr)
    {
        istringstream ss(dateTimeStr);
        tm t = {};
        ss >> get_time(&t, "%Y-%m-%d %H:%M:%S");
        return chrono::system_clock::from_time_t(mktime(&t));
    }

    // Função para adicionar filtros com base na escolha do usuário
    void adicionarFiltro()
    {
        char filtro;

        cout << "\n..:Selecione a opção de filtro desejada:.." << endl;
        cout << "a. Data/Hora" << endl;
        cout << "b. Código" << endl;
        cout << "c. Mensagem" << endl;
        cout << "d. Classificação" << endl;
        cout << "e. Prioridade" << endl;
        cout << "g. Protocolo" << endl;
        cout << "h. Endereço IP de Origem" << endl;
        cout << "i. Porta de Origem" << endl;
        cout << "j. Endereço IP de Destino" << endl;
        cout << "k. Porta de Destino" << endl;
        cout << "l. Cancelar" << endl;
        cout << "\nOpção: ";
        cin >> filtro;

        switch (filtro)
        {
        case 'a':
            adicionarFiltroDataHora();
            break;

        case 'b':
            adicionarFiltroCodigo();
            break;

        case 'c':
            adicionarFiltroMensagem();
            break;

        case 'd':
            adicionarFiltroClassificacao();
            break;

        case 'e':
            adicionarFiltroPrioridade();
            break;

        case 'g':
            adicionarFiltroProtocolo();
            break;

        case 'h':
            adicionarFiltroOrigemIP();
            break;

        case 'i':
            adicionarFiltroOrigemPorta();
            break;

        case 'j':
            adicionarFiltroDestinoIP();
            break;

        case 'k':
            adicionarFiltroDestinoPorta();
            break;

        case 'l':
            cout << "\nCancelando adição de filtro..." << endl;
            break;

        default:
            cout << "\nOpção Inválida. Por favor, tente novamente." << endl;
            break;
        }
    }

    void adicionarFiltroDataHora()
    {
        string entrada;
        cout << "\nDigite a Data/Hora (YYYY-MM-DD hh:mm:ss) ou Intervalo de Datas(YYYY-MM-DD hh:mm:ss/YYYY-MM-DD hh:mm:ss), sendo Data Mínima/Data Máxima: ";
        cin.ignore();
        getline(cin, entrada);

        stringstream ss(entrada);
        string dataHoraMin, dataHoraMax;
        getline(ss, dataHoraMin, '/');
        getline(ss, dataHoraMax, '/');

        chrono::system_clock::time_point start = analisarData(dataHoraMin);
        chrono::system_clock::time_point end = analisarData(dataHoraMax);

        funcoesDeFiltro.push_back([start, end, this](Registro *r)
                                  {
            chrono::system_clock::time_point registroDataHora = this->analisarData(r->getDataHora());
            return registroDataHora >= start && registroDataHora <= end; });

        descricaoFiltros.push_back("Data/Hora: " + dataHoraMin + " - " + dataHoraMax);
    }

    void adicionarFiltroCodigo()
    {
        string codigo;
        cout << "\nDigite o Código: ";
        cin.ignore();
        getline(cin, codigo);
        funcoesDeFiltro.push_back([codigo](Registro *r)
                                  { return r->getCodigo() == codigo; });
        descricaoFiltros.push_back("Código: " + codigo);
    }

    void adicionarFiltroMensagem()
    {
        string mensagem;
        cout << "\nDigite a Mensagem: ";
        cin.ignore();
        getline(cin, mensagem);
        transform(mensagem.begin(), mensagem.end(), mensagem.begin(), ::tolower); // Transforma em letra minúscula

        funcoesDeFiltro.push_back([mensagem](Registro *r)
                                  {
                                      string msgRegistro = r->getMensagem();
                                      transform(msgRegistro.begin(), msgRegistro.end(), msgRegistro.begin(), ::tolower); // Transforma em letra minúscula
                                      return msgRegistro.find(mensagem) != string::npos; });
        descricaoFiltros.push_back("Mensagem: " + mensagem);
    }

    void adicionarFiltroClassificacao()
    {
        string classificacao;
        cout << "\nDigite a Classificação: ";
        cin.ignore();
        getline(cin, classificacao);
        transform(classificacao.begin(), classificacao.end(), classificacao.begin(), ::tolower); // Transforma em letra minúscula

        funcoesDeFiltro.push_back([classificacao](Registro *r)
                                  {
            string classificacaoRegistro = r->getClassificacao();
            transform(classificacaoRegistro.begin(), classificacaoRegistro.end(), classificacaoRegistro.begin(), ::tolower);//Transforma em letra minúscula
            return classificacaoRegistro.find(classificacao) != string::npos; });
        descricaoFiltros.push_back("Classificação: " + classificacao);
    }

    void adicionarFiltroPrioridade()
    {
        string entrada;
        cout << "\nDigite a Prioridade (ou intervalo no formato min-max): ";
        cin.ignore();
        getline(cin, entrada);

        size_t pos = entrada.find('-');
        if (pos != string::npos)
        {
            int min = stoi(entrada.substr(0, pos));
            int max = stoi(entrada.substr(pos + 1));
            funcoesDeFiltro.push_back([min, max](Registro *r)
                                      { return r->getPrioridade() >= min && r->getPrioridade() <= max; });
            descricaoFiltros.push_back("Prioridade: " + to_string(min) + "-" + to_string(max));
        }
        else
        {
            int prioridade = stoi(entrada);
            funcoesDeFiltro.push_back([prioridade](Registro *r)
                                      { return r->getPrioridade() == prioridade; });
            descricaoFiltros.push_back("Prioridade: " + to_string(prioridade));
        }
    }

    void adicionarFiltroProtocolo()
    {
        string protocolo;
        cout << "\nDigite o Protocolo: ";
        cin.ignore();
        getline(cin, protocolo);
        transform(protocolo.begin(), protocolo.end(), protocolo.begin(), ::tolower); // Transforma em letra minúscula

        funcoesDeFiltro.push_back([protocolo](Registro *r)
                                  { 
            string protocoloRegistro = r->getProtocolo();
            transform(protocoloRegistro.begin(), protocoloRegistro.end(), protocoloRegistro.begin(), ::tolower);//Transforma em letra minúscula
            return protocoloRegistro == protocolo; });
        descricaoFiltros.push_back("Protocolo: " + protocolo);
    }

    void adicionarFiltroOrigemIP()
    {
        string origemIP;
        cout << "\nDigite o Endereço IP de Origem: ";
        cin.ignore();
        getline(cin, origemIP);
        funcoesDeFiltro.push_back([origemIP](Registro *r)
                                  { return r->getOrigemIP().find(origemIP) != string::npos; });
        descricaoFiltros.push_back("Endereço IP de Origem: " + origemIP);
    }

    void adicionarFiltroOrigemPorta()
    {
        string entrada;
        cout << "\nDigite a Porta de Origem (ou intervalo no formato min-max): ";
        cin.ignore();
        getline(cin, entrada);

        size_t pos = entrada.find('-');
        if (pos != string::npos)
        {
            int min = stoi(entrada.substr(0, pos));
            int max = stoi(entrada.substr(pos + 1));
            funcoesDeFiltro.push_back([min, max](Registro *r)
                                      { return r->getOrigemPorta() >= min && r->getOrigemPorta() <= max; });
            descricaoFiltros.push_back("Porta de Origem: " + to_string(min) + "-" + to_string(max));
        }
        else
        {
            int origemPorta = stoi(entrada);
            funcoesDeFiltro.push_back([origemPorta](Registro *r)
                                      { return r->getOrigemPorta() == origemPorta; });
            descricaoFiltros.push_back("Prioridade: " + to_string(origemPorta));
        }
    }

    void adicionarFiltroDestinoIP()
    {
        string destinoIP;
        cout << "\nDigite o Endereço IP de Destino: ";
        cin.ignore();
        getline(cin, destinoIP);
        funcoesDeFiltro.push_back([destinoIP](Registro *r)
                                  { return r->getDestinoIP().find(destinoIP) != string::npos; });
        descricaoFiltros.push_back("Endereço IP de Destino: " + destinoIP);
    }

    void adicionarFiltroDestinoPorta()
    {
        string entrada;
        cout << "\nDigite a Porta de Destino (ou intervalo no formato min-max): ";
        cin.ignore();
        getline(cin, entrada);

        size_t pos = entrada.find('-');
        if (pos != string::npos)
        {
            int min = stoi(entrada.substr(0, pos));
            int max = stoi(entrada.substr(pos + 1));
            funcoesDeFiltro.push_back([min, max](Registro *r)
                                      { return r->getDestinoPorta() >= min && r->getDestinoPorta() <= max; });
            descricaoFiltros.push_back("Porta de Destino: " + to_string(min) + "-" + to_string(max));
        }
        else
        {
            int destinoPorta = stoi(entrada);
            funcoesDeFiltro.push_back([destinoPorta](Registro *r)
                                      { return r->getDestinoPorta() == destinoPorta; });
            descricaoFiltros.push_back("Porta de Destino: " + to_string(destinoPorta));
        }
    }

    void limparFiltros() // Limpa os vetores de tipos de filtro e descricao dos filtros
    {
        cout << "\nLimpando todos os filtros..." << endl;
        funcoesDeFiltro.clear();
        descricaoFiltros.clear();
    }

    void visualizarFiltros() // Visualizar lista de filtros e descrição dos filtros
    {
        cout << "\nVisualizando todos os filtros..." << endl;
        if (!funcoesDeFiltro.size())
        {
            cout << "\nNenhum filtro aplicado no momento." << endl;
        }
        else
        {
            cout << "Filtros Aplicados:" << endl;
            for (const auto &descricao : descricaoFiltros)
            {
                cout << descricao << endl;
            }
            cout << "\nNúmero de filtros aplicados: " << funcoesDeFiltro.size() << endl;
        }
    }

    void visualizarDados() // Impressão dos Dados com filtros aplicados
    {
        cout << "\nVisualizando dados..." << endl;
        cout << left << setw(22) << "Date/Hour" << setw(12) << "Code" << setw(35) << " Message"
             << setw(35) << "Classification" << setw(12) << "Priority" << setw(12) << "Protocol"
             << setw(20) << "Source IP" << setw(15) << "Source Port" << setw(20) << "Destination IP" << setw(15) << "Destination Port" << endl;

        for (auto registro : logs)
        {
            if (aplicarFiltros(registro))
            {
                string mensagem = registro->getMensagem();
                if (mensagem.length() > 30)
                {
                    mensagem = mensagem.substr(0, 30);
                }

                string classificacao = registro->getClassificacao();
                if (classificacao.length()> 30)
                {
                    classificacao = classificacao.substr(0, 30);
                }

                cout << left << setw(22) << registro->getDataHora()
                     << setw(12) << registro->getCodigo()
                     << setw(35) << mensagem
                     << setw(35) << classificacao
                     << setw(12) << registro->getPrioridade()
                     << setw(12) << registro->getProtocolo()
                     << setw(20) << registro->getOrigemIP()
                     << setw(15) << (registro->getOrigemPorta() == 0 ? "" : to_string(registro->getOrigemPorta()))
                     << setw(20) << registro->getDestinoIP()
                     << setw(15) << (registro->getDestinoPorta() == 0 ? "" : to_string(registro->getDestinoPorta()))
                     << endl;
            }
        }

        if (!funcoesDeFiltro.size())
        {
            cout << "\nNenhum filtro aplicado no momento." << endl;
        }
        else
        {
            cout << "\nFiltros Aplicados:" << endl;
            for (const auto &descricao : descricaoFiltros)
            {
                cout << descricao << endl;
            }
            cout << "\nNúmero de filtros aplicados: " << funcoesDeFiltro.size() << endl;
        }
    }

    bool aplicarFiltros(Registro *registro) // Aplica os filtros para o vetor de dados
    {
        for (auto &filtro : funcoesDeFiltro)
        {
            if (!filtro(registro))
            {
                return false;
            }
        }
        return true;
    }

    void exportarDados() // Gera um arquivo com os dados com os filtros aplicados
    {
        cout << "\nExportando dados filtrados..." << endl;
        string nomeArquivo;

        cout << "\nInforme o nome do arquivo para salvar os registros: ";
        cin >> nomeArquivo;

        ofstream arquivo(nomeArquivo);
        if (!arquivo.is_open())
        {
            cout << "\nErro ao criar o arquivo." << endl;
            return;
        }

        arquivo << left << "Month\tDay\tHour\tCode\tMessage\tClassification\tPriority\tProtocol\tSource IP\tSource Port\tDestination IP\tDestination Port" << endl;

        for (auto registro : logs)
        {
            if (aplicarFiltros(registro))
            {

                arquivo << left << registro->getMes()
                        << "\t" << registro->getDia()
                        << "\t" << registro->getHora()
                        << "\t" << registro->getCodigo()
                        << "\t" << registro->getMensagem()
                        << "\t" << registro->getClassificacao()
                        << "\t" << registro->getPrioridade()
                        << "\t" << registro->getProtocolo()
                        << "\t" << registro->getOrigemIP()
                        << "\t" << (registro->getOrigemPorta() == 0 ? "" : to_string(registro->getOrigemPorta()))
                        << "\t" << registro->getDestinoIP()
                        << "\t" << (registro->getDestinoPorta() == 0 ? "" : to_string(registro->getDestinoPorta()))
                        << endl;
            }
        }
        arquivo.close();
        cout << "\nDados exportados com sucesso para " << nomeArquivo << "." << endl;
    }
};

int main()
{
    cout << "..:Iniciando Programa:.." << endl;

    string nomeArquivo;
    cout << "\nInforme o nome do arquivo a ser lido: ";
    cin >> nomeArquivo;

    Sistema sistema(nomeArquivo);

    int opcao;
    do
    {
        cout << "\n..:Escolha uma das opções abaixo:.." << endl;
        cout << "1. Adicionar Filtro" << endl;
        cout << "2. Limpar Filtros" << endl;
        cout << "3. Visualizar Filtros" << endl;
        cout << "4. Visualizar Dados" << endl;
        cout << "5. Exportar Dados" << endl;
        cout << "0. Sair" << endl;
        cout << "\nOpção: ";
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            sistema.adicionarFiltro();
            break;
        case 2:
            sistema.limparFiltros();
            break;
        case 3:
            sistema.visualizarFiltros();
            break;
        case 4:
            sistema.visualizarDados();
            break;
        case 5:
            sistema.exportarDados();
            break;
        case 0:
            cout << "\nSaindo..." << endl;
            break;
        default:
            cout << "\nOpção inválida. Por favor, tente novamente." << endl;
            break;
        }
    } while (opcao != 0);

    return 0;
}