# Filtragem Arquivos Snort em CPP
 
 Trabalho realizado na cadeira de Programação em C/C++ na UNISINOS pelo professor Marcio Miguel Gomes em 2024/1.

 Este projeto consiste em um sistema de gerenciamento de logs de segurança gerados pelo Snort, permitindo que o usuário filtre e visualize eventos de segurança com base em diferentes critérios, como data, código de erro, mensagem, prioridade, protocolo, entre outros. Além disso, o sistema permite exportar os dados filtrados para um arquivo de saída.

 Funcionalidades
 1. Criação de Registros de Log: A partir de um arquivo de entrada, cada linha é lida e convertida em um objeto Registro, contendo informações como data e hora, código de evento, mensagem, classificação, prioridade, protocolo, IP de origem e destino, e portas.

 2. Filtros de Dados: Os usuários podem adicionar filtros com base em vários critérios para visualizar somente os registros relevantes. Os filtros podem ser aplicados em:
 - Data/Hora (com suporte para intervalos)
 - Código
 - Mensagem
 - Classificação
 - Prioridade (com suporte para intervalos)
 - Protocolo
 - Endereço IP de Origem
 - Porta de Origem (com suporte para intervalos)
 - Endereço IP de Destino
 - Porta de Destino (com suporte para intervalos)
 
 3. Visualização de Filtros: Exibe todos os filtros aplicados atualmente, facilitando a análise e a compreensão dos dados filtrados.

 4. Visualização de Dados: Apresenta os dados filtrados, exibindo informações formatadas sobre cada evento de log que atende aos critérios de filtragem.

 5. Exportação de Dados: Permite exportar os dados filtrados para um arquivo, para posterior análise ou arquivamento.

 Para realização da tarefa foram utilizadas os seguintes conceitos de C++:
 - Tipos de Dados Básicos e Compostos: Utilização de tipos como string, int, e containers como vector e map.
 - Entrada e Saída de Dados: Manipulação de arquivos e leitura/escrita de dados.
 - Funções e Funções Lambda: Implementação de funcionalidades específicas, incluindo uso de funções lambda para filtros dinâmicos.
 - Ponteiros e Alocação Dinâmica de Memória: Utilização de ponteiros para gerenciamento de objetos Registro.
 - Passagem de Parâmetros por Referência e Valor: Melhoria de desempenho ao passar grandes objetos.
 - Gerenciamento de Recursos: Cuidados com a abertura e fechamento de arquivos, gerenciamento de memória e objetos dinâmicos.
 - Controle de Fluxo e Estruturas de Repetição: Implementação de loops e decisões condicionais para controle do fluxo do programa.
 - Manipulação de String e Data: Conversão e formatação de strings e datas.

 Este projeto demonstra a aplicação de conceitos fundamentais de programação em C++, oferecendo uma solução prática para o gerenciamento de logs de segurança, com a capacidade de filtrar, visualizar e exportar dados de maneira eficiente.