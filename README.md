# Cine-C: Sistema de Gestão e Reserva de Ingressos de Cinema

Projeto prático desenvolvido para a disciplina de **Programação Avançada**.
O sistema implementa o controle completo de salas, sessões de exibição, mapa dinâmico de assentos e emissão/cancelamento de reservas de ingressos via terminal em linguagem C.

---

## 👥 Equipe e Responsabilidades

- **Ana Beatriz**: Módulo de Sessões (`sessao.h` / `sessao.c`) — Modelagem do TAD `Sessao`, alocação dinâmica da matriz de assentos, visualização gráfica das fileiras e cálculo de ocupação da sala.
- **Matheus Henrique**: README e Módulo de Reservas (`reserva.h` / `reserva.c`) — Modelagem do TAD `Reserva`, vínculo entre cliente e assento, busca por código, controle de status e funcionalidade adicional de cancelamento em lote.
- **Luiza Vasconcelos**: Módulo de Persistência (`persistencia.h` / `persistencia.c`) — Leitura e gravação dos dados em arquivos de texto (`.txt`), parsing das linhas e recuperação do estado do sistema na inicialização.

---

## 📁 Estrutura do Projeto

O código-fonte segue a separação modular, isolando as interfaces (`include/`), implementações (`src/`) e arquivos de dados (`dados/`):

```text
reservas_cinema/
├── include/
│   ├── sessao.h          # Declaração dos tipos e operações do TAD Sessao
│   ├── reserva.h         # Declaração dos tipos e operações do TAD Reserva
│   └── persistencia.h    # Protótipos de gravação e leitura em arquivo
├── src/
│   ├── sessao.c          # Implementação e definição opaca da struct sessao
│   ├── reserva.c         # Implementação e definição opaca da struct reserva
│   ├── persistencia.c    # Funções de leitura e escrita com fopen/fprintf/fscanf
│   └── main.c            # Menu interativo e ponto de entrada do sistema
├── dados/
│   └── .gitkeep          # Mantém a pasta versionada no repositório
├── .gitignore            # Ignora os executáveis e arquivos de dados de teste
└── README.md             # Instruções e documentação do projeto
```

---

## ⚙️ Como Compilar e Executar

### 🐧 Opção 1: Linux, WSL ou macOS

*(Requer o compilador `gcc` instalado)*

1. **Compilar:**
No terminal, dentro da pasta raiz do projeto, execute:
```bash
gcc -Wall -Iinclude src/sessao.c src/reserva.c src/persistencia.c src/main.c -o cinema
```

2. **Executar:**
Após a compilação, inicie o sistema com:
```bash
./cinema
```

### 🪟 Opção 2: Windows nativo (PowerShell ou Prompt de Comando - CMD)

*(Requer MinGW/GCC configurado no PATH do Windows)*

1. **Compilar:**
```bash
gcc -Wall -Iinclude src/sessao.c src/reserva.c src/persistencia.c src/main.c -o cinema.exe
```

2. **Executar no PowerShell:**
```powershell
.\cinema.exe
```
*(Ou apenas `cinema.exe` se estiver usando o Prompt de Comando CMD)*

---

## 🚀 Funcionalidades Principais

1. **Cadastro de Sessões**: Criação de novas sessões com ID, título do filme, horário, sala e quantidade de linhas e colunas de assentos.
2. **Listagem de Sessões**: Exibição de todas as sessões cadastradas com informações de capacidade, assentos livres e percentual de ocupação.
3. **Mapa Gráfico de Assentos**: Desenho da sala no terminal identificando assentos livres `[ ]` e reservados `[X]`.
4. **Reserva de Assentos**: Validação de coordenadas, verificação de vaga livre e cadastro do cliente (nome e CPF).
5. **Cancelamento de Reserva**: Liberação imediata da poltrona no mapa e atualização do status da reserva.
6. **Consulta de Reservas**: Pesquisa de reservas ativas através do código identificador.
7. **Persistência de Dados**: Carregamento automático na abertura do programa e salvamento das alterações em disco.

---

## 🌟 Funcionalidade Adicional da Equipe

- **Cancelamento em Lote por Sessão (Esvaziamento Administrativo)**:
  - **Objetivo**: Permitir o cancelamento de todas as reservas ativas associadas a uma sessão específica de uma só vez (simulando casos reais de falha técnica no projetor, ar-condicionado ou cancelamento da exibição).
  - **Funcionamento**: O operador informa o ID da sessão e, após confirmação, o sistema percorre as reservas vinculadas, altera o status para cancelado e libera automaticamente todos os assentos correspondentes no mapa da sala.

---

## 💾 Armazenamento dos Dados

Os dados são armazenados na pasta `dados/` em formato de texto separado por ponto e vírgula (`;`):

- `dados/sessoes.txt`: `id;titulo;horario;sala;linhas;colunas`
- `dados/reservas.txt`: `codigo;id_sessao;cliente;cpf;linha;coluna;ativa`

Ao abrir o programa, as sessões são recriadas na memória com suas matrizes dinâmicas, e as reservas ativas remarcam as poltronas ocupadas.

---

## 🧠 Modelagem e Gerenciamento de Memória

- **Encapsulamento com TAD Opaco**: As estruturas `struct sessao` e `struct reserva` são definidas apenas nos arquivos `.c`. O restante do programa só manipula ponteiros e interage através das funções públicas disponibilizadas nos cabeçalhos (`.h`).
- **Matrizes Dinâmicas**: Cada sala aloca sua grade de assentos dinamicamente na Heap com ponteiro duplo (`int **`), suportando qualquer tamanho de fileiras e colunas.
- **Liberação de Memória**: Ao encerrar o sistema na opção de saída, todas as reservas e sessões (incluindo todas as linhas alocadas das matrizes) são liberadas com `free()`.