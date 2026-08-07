# Introdução

O sistema de exames fornece exercícios um por um e corrige-os quando solicitas.

Um exame é um conjunto de níveis, e cada nível tem seu conjunto de tarefas possíveis, dadas aleatoriamente. O teu colega provavelmente não terá o mesmo exerício que tu.

Para cada nível que validares, receberás pontos, até um máximo de 100.

## Antes do exame

Antes de fazer um exame, deverás inscrever-te (ou tentar novamente, se não for a primeira vez) em pelo menos um projeto de exame no Intra.

## Examshell

Poderás aceder ao sistema de exames usando um programa chamado `examshell`

Deverás inicia-lo no teu terminal, o mais tardar 10 minutos após a data de início do exame. Se parares inadvertidamente, não existe problema. Poderás iniciá-lo novamente digitando `examshell` no terminal. Este irá retomar o processo para onde estava antes.

# Versão curta

1. Inicia o examshell
2. Examshell informa os diferentes exames aos quais você tem acesso
3. O sistema cria um repositório Git e o clona para BASEDIR/rendu
4. O sistema escolhe um exercício aleatório para fazer no nível atual
5. Examshell faz o download do suject para as atribuições para BASEDIR/subjects/ASSIGNMENT_NAME/
6. Examshell indica qual o exercício a fazer, quantos pontos podes ganhar e onde deves entregá-lo
7. Trabalha no teu exercício
8. Envia o exercício terminado para o repositório Git como de costume
9. Quando tiveres a certeza que terminaste: Diz ao examshell para avaliar o exercício usando o comando `grademe`
10. O sistema irá avaliar o teu exercício
11. Examshell salva os traces da correção em BASEDIR/traces/ASSIGNMENT_TRACE_DIR
12. Se fores bem sucedido, ganharás pontos e avançarás para o nível seguinte
13. Se reprovares, permanecerás no mesmo exercício.

Não entendeste? Lê a versão longa!

# Versão longa


## Etapa 1: iniciando o examshell

```
zaz@blackjack ~ $ examshell
examshell v0.1.1

WARNING
Your exam files will be stored in ~/exam-basedir
THIS DIRECTORY WILL BE ENTIRELY EMPTIED BEFORE YOU START
So, if you do have important things there, Ctrl-C NOW and back them up before running this.
(Press Enter to continue...)
```

## Etapa 2: Preparando o pasta de Trabalho

Depois de confirmar, o examshell preparará uma pasta de trabalho, clonará o repositório Git apropriado, etc ...

```
[...]
Confirm ? [y/n] y
Selecting project...
Creating required directories...
Ensuring your Git repository for this exam is present and correct...
Git repository is not cloned yet. Cloning...
Cloning into '/Users/zaz/exam-basedir/rendu'...
vogsphere: (INFO) Transaction ID : 8ed938b3-fe1e-4eb6-b561-0f6622e12b82
vogsphere: (INFO) Please mention this ID in any ticket you create concerning this transaction
vogsphere: (INFO) This transaction has been started at 2015-05-28 11:07:49, server time.
vogsphere: (INFO) Rights will be determined using this time, so do NOT cut the connection.
vogsphere: (INFO) It appears you are mmontinet. If that's not true, check your Kerberos tickets (klist)
vogsphere: (INFO) You have read and write rights on this repository
warning: You appear to have cloned an empty repository.

Your git repository was successfully cloned to ~/exam-basedir/rendu

The following commands are available to you:
  status: Displays the status of your session, including information about
    your current assignment, and the exam history.
  grademe: Asks the server to grade your current assignment. If you
    have done it right, you will gain the points of the current assignment, go
    up a level, and try the next one. If you fail, however, you will have
    another assignment of the same level to do, and it will potentially bring
    you less points on your grade ... So be sure of yourself before you launch
    this command !
  finish: Tells the server you are finished with your exam.

You can log out at any time. If this program tells you you earned points,
then they will be counted whatever happens.

(Press Enter to continue...)
```

## Etapa 4: Obter um exercício

O Examshell vai atribuir-te um exercício do sistema de exames

Ele salvará automaticamente o subject na pasta `subjects`. Podes lê-lo no terminal.

```
[...]
================================================================================
You are currently at level 0
You are running in practice mode (Your grade does not count)
Your current grade is 0/100
Assignments:
  Level 0:
    0: max for 16 potential points (Current)

Your current assignment is max for 16 potential points
It is assignment 0 for level 0
The subject is located at: ~/exam-basedir/subjects/max
You must turn in your files in a subdirectory of your Git repository with the
same name as the assignment (~/exam-basedir/rendu/max).
Of course, you must still push...

The end date for this exam is: 28/05/2015 15:07:47
You have 3 hours, 54 minutes and 11 seconds remaining
================================================================================
You can now work on your assignment. When you are sure you're done with it,
push it to vogsphere, and then use the "grademe" command to be graded.
examshell>
```

Esta informação está sempre acessível usando o comando `status`.

## Etapa 5: trabalhe em sua tarefa

Agora, bem, podes trabalhar no exercío que te foi atríbuido.
Terás de entregar o teu exercício na pasta indicada pelo examshell.
Neste exemplo, colocaríamos nosso exercício `max.c` em `~/exam-basedir/rendu/max/`.

Terás que dar push no teu trabalho depois de terminar, como em qualquer projeto. Não te esqueças!

## Etapa 6: solicitar a correção

Depois de ter certeza terminaste o exercício e enviaste para o vogsphere, podes usar o comando `grademe` para solicitar que o teu exercício seja corrigido.

```
examshell> grademe

Before continuing, please make ABSOLUTELY SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will have another assignment at the same level,
but with less potential points to earn!

Are you sure? [y/N]
```

Então, sim, diz que tens a certeza.

```
[...]

Are you sure? [y/N] y
OK, making grading request to server now.

We will now wait for the job to complete.
Please be patient, this CAN take several minutes...
(10 seconds is fast, 30 seconds is expected, 3 minutes is a maximum)
waiting...
```

Agora esperamos que o sistema corrija o exercício. PODE levar algum tempo; portanto, não entre em pânico, a menos que esperes mais de 1-2 minutos. Nesse caso, contacta um membro do staff. A solução é, normalmente, fácil.

### Sucesso !

```
[...]
waiting...
>>>>>>>>>> SUCCESS <<<<<<<<<<
You have successfully completed the assignment and earned 16 points!
Trace saved to ~/exam-basedir/traces/0-0_max.trace

(Press Enter to continue...)
```

Nesse caso, conseguiste acertar!

A próxima mensagem de status mostrará que:

* Ganhaste os pontos do exercício em questão
* Subiste um nível
* Tens um novo exercício para realizar

```
[...]
(Press Enter to continue...)

================================================================================
You are currently at level 1
You are running in practice mode (Your grade does not count)
Your current grade is 16/100
Assignments:
  Level 0:
    0: max for 16 potential points (Success)
  Level 1:
    0: wdmatch for 16 potential points (Current)

Your current assignment is wdmatch for 16 potential points
It is assignment 0 for level 1
The subject is located at: ~/exam-basedir/subjects/wdmatch
You must turn in your files in a subdirectory of your Git repository with the
same name as the assignment (~/exam-basedir/rendu/wdmatch).
Of course, you must still push...

The end date for this exam is: 28/05/2015 15:07:47
You have 3 hours, 42 minutes and 27 seconds remaining
================================================================================
You can now work on your assignment. When you are sure you're done with it,
push it to vogsphere, and then use the "grademe" command to be graded.
examshell>
```

Se esse tivesse sido o último nível do exame, o examshell teria dito e avisado que o exame estava terminado.

### Falha :(

Vamos falhar neste exercício, apenas para ver o que o examshell apresenta:

```
[...]
examshell> grademe

Before continuing, please make ABSOLUTELY SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will have another assignment at the same level,
but with less potential points to earn!

Are you sure? [y/N] y
OK, making grading request to server now.

We will now wait for the job to complete.
Please be patient, this CAN take several minutes...
(10 seconds is fast, 30 seconds is expected, 3 minutes is a maximum)
waiting...
>>>>>>>>>> FAILURE <<<<<<<<<<
You have failed the assignment.
Trace saved to ~/exam-basedir/traces/1-0_wdmatch.trace

(Press Enter to continue...)

================================================================================
You are currently at level 1
You are running in practice mode (Your grade does not count)
Your current grade is 16/100
Assignments:
  Level 0:
    0: max for 16 potential points (Success)
  Level 1:
    0: wdmatch for 16 potential points (Failure)
    1: wdmatch for 16 potential points (Current)

Your current assignment is ft_strrev for 11 potential points
It is assignment 1 for level 1
The subject is located at: ~/exam-basedir/subjects/ft_strrev
You must turn in your files in a subdirectory of your Git repository with the
same name as the assignment (~/exam-basedir/rendu/ft_strrev).
Of course, you must still push...

The end date for this exam is: 28/05/2015 15:07:47
You have 3 hours, 41 minutes and 25 seconds remaining
================================================================================
You can now work on your assignment. When you are sure you're done with it,
push it to vogsphere, and then use the "grademe" command to be graded.
examshell>
```

Nesse caso:

* NÃO obtes os pontos
* NÃO sobes de nível (permanecemos no 1)
* Temos o mesmo exercício com os mesmos pontos para ganhar!


### Erro :<

Existe a possibilidade de que o exame diga que a correção resultou em um ERRO. Isso não significa que falhaste, significa que o próprio sistema falhou.

No entanto, não entres em pânico!

Terás a opção de tentar novamente ou abortar. Deverás tentar pelo menos uma vez, porque os erros no sistema de correção são raros e geralmente transitórios. Se, após uma nova tentativa não resolver o erro, pede ajuda a um membro do bocal

## Etapa 7: É isso!

Irás continuar a receber novas tarefas até que a data do exame termine ou sejas bem-sucedido no último nível do exame.

Poderás encerrar sua sessão a qualquer momento usando o comando `finish`.

```
examshell> finish
Please confirm that you REALLY want to end your current session.
If you do, you will not be able to do anything with it anymore!
Are you finished? [y/N] y
Your session has been marked as finished. You may now log out.
zaz@blackjack ~ $
```

# FAQ / Erros comuns

## Eu fechei o examshell, o que devo fazer?

Basta abrir de novo.

## Examshell diz "login window expired"

Tens 10 minutos para escolher um exame após a data e horário de início do exame. Depois, desses 10 minutos, receberás esse erro porque é tarde demais.

## Examshell diz "Mismatched versions"

O computador onde estás não está atualizado. Muda para outro computador após indicar o problema a um membro do staff.

## Eu não tinha os mesmo exercícios do meu colega, isso é injusto!

Sim. Que pena. Lida com isso.

## Examshell me disse que falhei, mas ei tenho certeza que não!

Muito provavelmente, esqueceste-te de algo:

* Deste push ?
* A sério ?
* Usaste a pasta certa?
* Os teus exercícios estão com o nome correto?
* Fizeste o exercício correto?
* Tens a CERTEZA que deste push ?
* Etc...

Se estás realmente, realmente, REALMENTE, ABSOLUTAMENTE certo de que fizeste tudo certo, e que o sistema te prejudicou, diz à equipe APÓS O EXAME. Vamos dar uma olhada, mas por favor, por favor, pensa antes de falar connosco.

## Eu encontrei um bug no sistema!

A sério?

Se sim, por favor conta-nos, vamos ficar felizes.

## Eu encontrei um bug no sistema e eu o usei isso a meu favor!

Em algum momentos nós iremos descobrir. E esperamos que gostes de procurar outra escola :)

Basta denunciá-lo e seguir em frente. Não vale a pena ser expulso por alguns pontos em um exame.

## Fiz um pedido de avaliação de exercício, mas esta a demorar muito tempo

Se não terminar depois de, digamos, um ou dois minutos, fala com um membro da equipa, porque realmente não é normal. Normalmente é uma solução fácil para resolvermos.