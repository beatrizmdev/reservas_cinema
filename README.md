# Cine-C: Sistema de Gestão e Reserva de Ingressos de Cinema

Projeto prático desenvolvido para a disciplina de **Programação Avançada** (Semestre 2026.2).
O sistema implementa o controle completo de salas, sessões de exibição, mapa dinâmico de assentos e emissão/cancelamento de reservas de ingressos via terminal em linguagem C.

---

## 👥 Equipe e Responsabilidades

- **Ana Medeiros**: Módulo de Sessões (`sessao.h` / `sessao.c`) — Modelagem do TAD `Sessao`, alocação dinâmica da matriz bidimensional de assentos, visualização gráfica das fileiras e cálculo de ocupação.
- **Matheus Henrique**: README e Módulo de Reservas (`reserva.h` / `reserva.c`) — Modelagem do TAD `Reserva`, vínculo cliente-poltrona, busca por código, controle de status e funcionalidade adicional de cancelamento em lote.
- **Luiza Vasconcelos**: Módulo de Persistência (`persistencia.h` / `persistencia.c`) — Serialização e recuperação de dados em arquivos de texto (`.txt`), parsing seguro de strings e restauração de ponteiros e matrizes na inicialização.

---

## 📁 Estrutura do Projeto

O código-fonte segue estritamente a separação modular recomendada, isolando interfaces públicas (`include/`), implementações e regras de negócio (`src/`) e arquivos de dados (`dados/`):

```text
reservas_cinema/
├── include/
│   ├── sessao.h          # Interface pública e protótipos do TAD Sessao
│   ├── reserva.h         # Interface pública e protótipos do TAD Reserva
│   └── persistencia.h    # Funções de gravação e leitura em disco
├── src/
│   ├── sessao.c          # Implementação e definição opaca da struct sessao
│   ├── reserva.c         # Implementação e definição opaca da struct reserva
│   ├── persistencia.c    # Manipulação de arquivos (fopen, fprintf, fscanf)
│   └── main.c            # Ponto de entrada, menu interativo e gerenciamento de memória
├── dados/
│   └── .gitkeep          # Garante o versionamento do diretório de dados
├── .gitignore            # Ignora binários gerados e arquivos de dados locais
├── Makefile              # Automação de compilação
└── README.md             # Documentação técnica do sistema
```

---

## ⚙️ Como Compilar e Executar

O projeto foi desenvolvido sob o padrão C99 e utiliza flags rigorosas de compilação (`-Wall -Wextra`) para garantir conformidade e ausência de warnings.

### Pré-requisitos
- Ambiente Linux, WSL (Windows Subsystem for Linux) ou macOS.
- Compilador `gcc`.
- Utilitário `make` (opcional).

### Opção 1: Compilação via Linha de Comando (GCC direta)
Para compilar e vincular todos os módulos:
```bash
gcc -Wall -Wextra -std=c99 -Iinclude src/sessao.c src/reserva.c src/persistencia.c src/main.c -o cinema
```

Para executar o sistema:
```bash
./cinema
```

### Opção 2: Compilação via Makefile
Caso possua o `make` instalado:
```bash
# Compilar o executável
make

# Compilar e executar imediatamente
make run

# Limpar binários gerados
make clean
```

---

## 🚀 Funcionalidades do Sistema

1. **Cadastro de Sessões**: Registro de novas sessões informando ID único, título do filme, horário, sala e dimensões configuráveis de assentos (linhas e colunas).
2. **Listagem de Sessões**: Exibição tabular de todas as sessões cadastradas com total de assentos vagos e taxa percentual de ocupação.
3. **Mapa Gráfico de Assentos**: Renderização visual no terminal da sala de cinema, indicando assentos livres `[ ]` e ocupados `[X]` com índices alinhados de fileiras e colunas.
4. **Reserva de Assentos**: Validação de coordenadas, verificação de disponibilidade prévia e emissão de reserva associada a código, nome do cliente e CPF.
5. **Cancelamento de Reserva**: Liberação imediata da poltrona no mapa e alteração do status da reserva para cancelada.
6. **Consulta de Reservas**: Pesquisa de reservas registradas pelo código identificador.
7. **Persistência Automática**: Carregamento de dados na inicialização e salvamento automático das alterações ao encerrar o sistema.

---

## 🌟 Funcionalidade Adicional (Requisito Plus da Equipe)

- **Cancelamento em Lote por Sessão (Esvaziamento Administrativo de Sala)**:
  - **Motivação**: Em um cinema real, incidentes técnicos (pane no projetor, falha no ar-condicionado ou cancelamento da exibição) exigem que todas as reservas daquela sessão específica sejam canceladas em lote de forma ágil, liberando as poltronas e estornando os clientes.
  - **Implementação Técnica**: A função `reserva_cancelar_por_sessao` percorre a coleção de reservas ativas, identifica todas as associadas ao ID da sessão informada, altera o status para inativo e desmarca os assentos correspondentes na matriz da `Sessao`.

---

## 💾 Persistência de Dados

Os dados são salvos em formato de texto delimitado por ponto e vírgula (`;`), localizado na pasta `dados/`:

- `dados/sessoes.txt`:
  - **Formato**: `id;titulo;horario;sala;linhas;colunas`
  - **Exemplo**: `1;Interestelar;19:30;1;4;5`
- `dados/reservas.txt`:
  - **Formato**: `codigo;id_sessao;cliente;cpf;linha;coluna;ativa`
  - **Exemplo**: `101;1;Carlos Silva;123.456.789-00;1;2;1`

Na inicialização, o sistema lê as sessões, aloca dinamicamente cada mapa na Heap e, em seguida, lê as reservas ativas, marcando automaticamente com `[X]` as poltronas ocupadas.

---

## 🧠 Arquitetura de Memória e Boas Práticas

- **Tipos Abstratos de Dados (TAD) Opacos**: As definições das estruturas `struct sessao` e `struct reserva` encontram-se exclusivamente dentro de seus respectivos arquivos `.c`. O acesso aos campos é feito exclusivamente por funções públicas (getters e setters), garantindo encapsulamento estrito.
- **Alocação Dinâmica na Heap**: A matriz de assentos de cada sessão é alocada dinamicamente via ponteiro duplo (`int **`), permitindo que cada sala possua dimensões arbitrárias de linhas e colunas.
- **Gerenciamento de Memória sem Vazamentos (Zero Memory Leaks)**: Ao sair do sistema, todas as instâncias de `Reserva` e de `Sessao` (incluindo todas as linhas da matriz de assentos) são devidamente desalocadas com `free()` através de `reserva_deletar` e `sessao_deletar`.