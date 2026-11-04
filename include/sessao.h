typedef struct sessao Sessao;

Sessao* sessao_criar(int id, const char *titulo, const char *horario, int sala, int linhas,
int colunas);

void sessao_deletar(Sessao *s);

int sessao_obter_id(const Sessao *s);

int sessao_obter_assentos_vagos(const Sessao *s);

float sessao_obter_taxa_ocupacao(const Sessao *s);

void sessao_exibir_mapa(const Sessao *s);

int sessao_assento_livre(const Sessao *s, int linha, int coluna);

int sessao_marcar_assento(Sessao *s, int linha, int coluna, int estado);

void sessao_listar_todas(Sessao **sessoes, int qtd_sessoes);

Sessao* sessao_buscar_por_id(Sessao **sessoes, int qtd_sessoes, int id);