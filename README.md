# trabalho-final-sm66a
Projeto FInal da disciplina de Sistemas Microcontrolados

# Jogo Simon utilizando o microcontolador TM4C123G

## 📝 Descrição Técnica

Este repositório contém o desenvolvimento de um sistema embarcado do Jogo Simon, utilizando o microcontrolador TM4C123G. O projeto foi desenvolvido como parte dos requisitos avaliativos da disciplina SM66A - Sistemas Microcontrolados.

O objetivo do projeto é implementar um jogo de memória no qual o microcontrolador gera uma sequência crescente de LEDs acesos e sons associados. O jogador deve repetir a sequência correta utilizando botões. O sistema exibe a pontuação em displays de 7 segmentos, além de fornecer feedback sonoro para erros e reinício.

## ✨ Funcionalidades Implementadas

- Geração de sequência para os LEDs.
- Reconhecimento de entrada do jogador via botões.
- Feedback sonoro com diferentes sons para cada LED e eventos (erro e reinício).
- Contagem de pontos exibida em displays de 7 segmentos.
- Aumento progressivo da dificuldade conforme o jogador acerta.

## ✨ Periféricos Utilizados

- GPIO: Controle dos LEDs.
- GPIO: Leitura dos botões.
- GPIO: Acionamento dos displays de 7 segmentos.
- PWM: Geração de sons diferenciados para os LEDs, acertos, erros e reinício.
- TIMER:
      Temporização de eventos no jogo (tempo dos LEDs, debouncing dos botões).
      Multiplexação dos displays de 7 segmentos.

## 🛠️ Hardware e Componentes

* Microcontrolador: TM4C123G
* Botões: 4 push-buttons (entrada do jogador)
* LEDs: 4 unidades (indicadores de sequência)
* Displays de 7 segmentos: 2 dígitos (contagem de pontos)
* Buzzer piezoelétrico ou alto-falante pequeno: Para emissão dos sons
* Resistores: Limitação de corrente para LEDs, displays e buzzer
* Protoboard e jumpers: Montagem física

## ⚙️ Procedimento de Montagem e Execução

*(Esta seção deve ser detalhada ao longo do desenvolvimento)*

1.  **Diagrama Elétrico:** O esquema de interconexão dos componentes está disponível em `docs/esquema.pdf`.
2.  **Configuração do Ambiente:** Detalhamento das bibliotecas e configurações da IDE necessárias.
3.  **Compilação e Upload:** Instruções para compilar o firmware e transferi-lo para o microcontrolador.

## 🚀 Cronograma e Evolução (Roadmap)

- [ ] Definição da arquitetura de software.
- [ ] Implementação do módulo de leitura de sensores.
- [ ] Implementação da lógica de controle.
- [ ] Validação e testes funcionais.
