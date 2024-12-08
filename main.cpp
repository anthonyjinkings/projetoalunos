#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <array>
#include <algorithm>

using namespace std;

struct Aluno {
    string nome;
    int matricula;
    string curso;
    int anoMatricula;
    map<string, pair<array<float, 3>, int>> materias;  
};

struct Materia {
    string nome;
    string codigo;
    int creditos;
    vector<int> alunosMatriculados;  
};

struct Curso {
    string nome;
    vector<Aluno*> alunosMatriculados;
};

Aluno* buscarAlunoPorMatricula(const vector<Aluno*>& alunos, int matricula) {
    int low = 0, high = alunos.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (alunos[mid]->matricula == matricula) {
            return alunos[mid];
        } else if (alunos[mid]->matricula < matricula) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return nullptr;
}

Materia* buscarMateriaPorCodigo(const vector<Materia>& materias, const string& codigo) {
    int low = 0, high = materias.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (materias[mid].codigo == codigo) {
            return const_cast<Materia*>(&materias[mid]);
        } else if (materias[mid].codigo < codigo) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return nullptr;
}

void realizarMatricula(vector<Aluno*>& alunos, vector<Curso>& cursos) {
    string nome, curso;
    int matricula, anoMatricula;

    cout << "Digite o nome do aluno: ";
    cin.ignore();
    getline(cin, nome);
    cout << "Digite a matrícula do aluno: ";
    cin >> matricula;
    cout << "Digite o ano de matrícula: ";
    cin >> anoMatricula;
    cout << "Digite o nome do curso em que deseja se matricular: ";
    cin.ignore();
    getline(cin, curso);

    Aluno* novoAluno = new Aluno;
    novoAluno->nome = nome;
    novoAluno->matricula = matricula;
    novoAluno->curso = curso;
    novoAluno->anoMatricula = anoMatricula;

    bool cursoEncontrado = false;
    for (Curso& c : cursos) { 
        if (c.nome == curso) {
            c.alunosMatriculados.push_back(novoAluno);
            cursoEncontrado = true;
            break;
        }
    }

    if (!cursoEncontrado) {
        Curso novoCurso;
        novoCurso.nome = curso;
        novoCurso.alunosMatriculados.push_back(novoAluno);
        cursos.push_back(novoCurso);
    }

    alunos.push_back(novoAluno);
    cout << "Matrícula realizada com sucesso!\n";
}

void listarRankingPorMateria(const vector<Materia>& materias, const vector<Aluno*>& alunos) {
    string codigoMateria;
    cout << "Digite o código da matéria para listar o ranking: ";
    cin >> codigoMateria;

    Materia* materia = buscarMateriaPorCodigo(materias, codigoMateria);
    if (materia == nullptr) {
        cout << "Matéria não encontrada!\n";
        return;
    }

    vector<pair<string, float>> ranking;
    for (int matricula : materia->alunosMatriculados) {
        Aluno* aluno = buscarAlunoPorMatricula(alunos, matricula);
        if (aluno != nullptr) {
            if (aluno->materias.find(codigoMateria) != aluno->materias.end()) {
                float somaNotas = 0;
                for (int i = 0; i < 3; ++i) {
                    somaNotas += aluno->materias[codigoMateria].first[i];
                }
                float media = somaNotas / 3.0;
                ranking.push_back(make_pair(aluno->nome, media));
            }
        }
    }

    for (size_t i = 0; i < ranking.size() - 1; ++i) {
        size_t maxIndex = i;
        for (size_t j = i + 1; j < ranking.size(); ++j) {
            if (ranking[j].second > ranking[maxIndex].second) {  
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            swap(ranking[i], ranking[maxIndex]);  
        }
    }

    cout << "\n===== Ranking da Matéria =====\n";
    for (const auto& entry : ranking) {
        cout << entry.first << " - Média: " << entry.second << "\n";
    }
}

void listarMateriasAluno(const vector<Aluno*>& alunos) {
    int matricula;
    cout << "Digite a matrícula do aluno para ver as matérias cursadas: ";
    cin >> matricula;

    Aluno* aluno = buscarAlunoPorMatricula(alunos, matricula);
    if (aluno == nullptr) {
        cout << "Aluno não encontrado!\n";
        return;
    }

    cout << "\nAluno: " << aluno->nome << "\n";
    cout << "Matérias: \n";
    for (const auto& materia : aluno->materias) {
        cout << " - " << materia.first << "\n";
    }
}

void verDetalhesMatriculaAluno(const vector<Aluno*>& alunos) {
    int matricula;
    cout << "Digite a matrícula do aluno para ver os detalhes da matrícula: ";
    cin >> matricula;

    Aluno* aluno = buscarAlunoPorMatricula(alunos, matricula);
    if (aluno == nullptr) {
        cout << "Aluno não encontrado!\n";
        return;
    }

    cout << "\nDetalhes da Matrícula do Aluno: \n";
    cout << "Nome: " << aluno->nome << "\n";
    cout << "Matrícula: " << aluno->matricula << "\n";
    cout << "Curso: " << aluno->curso << "\n";
    cout << "Ano da Matrícula: " << aluno->anoMatricula << "\n";
}

void cadastrarMateria(vector<Materia>& materias) {
    string nome, codigo;
    int creditos;

    cout << "Digite o nome da matéria: ";
    cin.ignore();
    getline(cin, nome);
    cout << "Digite o código da matéria: ";
    cin >> codigo;
    cout << "Digite o número de créditos da matéria: ";
    cin >> creditos;

    Materia novaMateria;
    novaMateria.nome = nome;
    novaMateria.codigo = codigo;
    novaMateria.creditos = creditos;

    materias.push_back(novaMateria);
    cout << "Matéria cadastrada com sucesso!\n";
}

void vincularAlunoAMateria(vector<Aluno*>& alunos, vector<Materia>& materias) {
    int matriculaAluno;
    string codigoMateria;

    cout << "Digite a matrícula do aluno: ";
    cin >> matriculaAluno;
    cout << "Digite o código da matéria: ";
    cin >> codigoMateria;

    Aluno* aluno = buscarAlunoPorMatricula(alunos, matriculaAluno);
    Materia* materia = buscarMateriaPorCodigo(materias, codigoMateria);

    if (aluno == nullptr) {
        cout << "Aluno não encontrado!\n";
        return;
    }
    if (materia == nullptr) {
        cout << "Matéria não encontrada!\n";
        return;
    }

    materia->alunosMatriculados.push_back(matriculaAluno);
    aluno->materias[codigoMateria] = make_pair(array<float, 3>{0, 0, 0}, 0);  
    cout << "Aluno matriculado na matéria com sucesso!\n";
}

void cadastrarNotasEFrequencias(vector<Aluno*>& alunos, vector<Materia>& materias) {
    int matriculaAluno;
    string codigoMateria;
    array<float, 3> notas;
    int frequencia;

    cout << "Digite a matrícula do aluno: ";
    cin >> matriculaAluno;
    cout << "Digite o código da matéria: ";
    cin >> codigoMateria;

    Aluno* aluno = buscarAlunoPorMatricula(alunos, matriculaAluno);
    Materia* materia = buscarMateriaPorCodigo(materias, codigoMateria);

    if (aluno == nullptr) {
        cout << "Aluno não encontrado!\n";
        return;
    }
    if (materia == nullptr) {
        cout << "Matéria não encontrada!\n";
        return;
    }

    cout << "Digite as 3 notas do aluno: ";
    for (int i = 0; i < 3; ++i) {
        cin >> notas[i];
    }

    cout << "Digite a frequência do aluno (em %): ";
    cin >> frequencia;

    aluno->materias[codigoMateria] = make_pair(notas, frequencia);
    cout << "Notas e frequência cadastradas com sucesso!\n";
}

void listarAlunosAprovadosReprovadosPorNotaOuFrequencia(const vector<Materia>& materias, const vector<Aluno*>& alunos) {
    string codigoMateria;
    cout << "Digite o código da matéria para listar os aprovados/reprovados: ";
    cin >> codigoMateria;

    Materia* materia = buscarMateriaPorCodigo(materias, codigoMateria);
    if (materia == nullptr) {
        cout << "Matéria não encontrada!\n";
        return;
    }

    for (int matricula : materia->alunosMatriculados) {
        Aluno* aluno = buscarAlunoPorMatricula(alunos, matricula);
        if (aluno != nullptr) {
            if (aluno->materias.find(codigoMateria) != aluno->materias.end()) {
                pair<array<float, 3>, int> dados = aluno->materias[codigoMateria];
                float media = (dados.first[0] + dados.first[1] + dados.first[2]) / 3.0;
                int frequencia = dados.second;

                cout << aluno->nome << " - Média: " << media << " - Frequência: " << frequencia << "%\n";
                if (media >= 6.0 && frequencia >= 75) {
                    cout << "Aprovado!\n";
                } else {
                    cout << "Reprovado!\n";
                }
            }
        }
    }
}

void listarAlunosReprovadosEmMaterias(const vector<Aluno*>& alunos) {
    cout << "===== Listagem de Alunos Reprovados =====\n";
    for (Aluno* aluno : alunos) {
        cout << "Aluno: " << aluno->nome << "\n";
        for (const auto& materia : aluno->materias) {
            float media = (materia.second.first[0] + materia.second.first[1] + materia.second.first[2]) / 3.0;
            int frequencia = materia.second.second;

            if (media < 6.0 || frequencia < 75) {
                cout << " - Matéria: " << materia.first << " - Média: " << media << " - Frequência: " << frequencia << "% (Reprovado)\n";
            }
        }
    }
}

void menuProfessor(vector<Aluno*>& alunos, vector<Materia>& materias, vector<Curso>& cursos) {
    int opcao;
    do {
        cout << "\n===== Menu Professor =====\n";
        cout << "1. Cadastrar Matéria\n";
        cout << "2. Vincular Aluno a Matéria\n";
        cout << "3. Cadastrar Notas e Frequências\n";
        cout << "4. Listar Alunos Aprovados/Reprovados por Nota ou Frequência\n";
        cout << "5. Listar Alunos Reprovados em Matérias\n";
        cout << "6. Voltar ao Menu Principal\n";
        cout << "Digite a opção: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cadastrarMateria(materias);
                break;
            case 2:
                vincularAlunoAMateria(alunos, materias);
                break;
            case 3:
                cadastrarNotasEFrequencias(alunos, materias);
                break;
            case 4:
                listarAlunosAprovadosReprovadosPorNotaOuFrequencia(materias, alunos);
                break;
            case 5:
                listarAlunosReprovadosEmMaterias(alunos);
                break;
            case 6:
                cout << "Voltando ao Menu Principal...\n";
                break;
            default:
                cout << "Opção inválida.\n";
        }
    } while (opcao != 6);
}

int main() {
    vector<Aluno*> alunos;
    vector<Materia> materias;
    vector<Curso> cursos;
    int opcao;

    do {
        cout << "\n===== Menu Principal =====\n";
        cout << "1. Menu Aluno\n";
        cout << "2. Menu Professor\n";
        cout << "3. Sair\n";
        cout << "Digite a opção: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                int opcaoAluno;
                do {
                    cout << "\n===== Menu Aluno =====\n";
                    cout << "1. Realizar Matrícula\n";
                    cout << "2. Listar Ranking por Matéria\n";
                    cout << "3. Listar Matérias Cursadas\n";
                    cout << "4. Ver Detalhes da Matrícula\n";
                    cout << "5. Voltar\n";
                    cout << "Digite a opção: ";
                    cin >> opcaoAluno;

                    switch (opcaoAluno) {
                        case 1:
                            realizarMatricula(alunos, cursos);
                            break;
                        case 2:
                            listarRankingPorMateria(materias, alunos);
                            break;
                        case 3:
                            listarMateriasAluno(alunos);
                            break;
                        case 4:
                            verDetalhesMatriculaAluno(alunos);
                            break;
                        case 5:
                            cout << "Voltando ao Menu Principal...\n";
                            break;
                        default:
                            cout << "Opção inválida.\n";
                    }
                } while (opcaoAluno != 5);
                break;
            case 2:
                menuProfessor(alunos, materias, cursos);
                break;
            case 3:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opção inválida.\n";
        }
    } while (opcao != 3);

    return 0;
}
