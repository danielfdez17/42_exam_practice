# Introduction

Examのシステムはあなたに課題を一つずつアサインし、あなたが評価を要求した際に応じて評価を行います。
Examには色々なレベルがあり、アサインされる課題が各レベルごとにランダムに決められます。あなたの隣にいる人はあなたと同じ課題をこなしているとは限りません。
人生は平等ではありません、受け入れなさい。
課題をクリアするごとに得点を獲得していき、最高点は100点です。

## examが始まる前に

examを開始する前に、イントラネットでexamプロジェクトに登録（か初めてではない場合、リトライ）しなければなりません。
好きなだけ登録できます、リミットはありません。

一つでもexamプロジェクトに登録すれば、`examshell`を用いてexamにアクセスできます。

リアルモードで実行したい場合、examセッションに登録する必要があります。examのセッション時には、その場にいる必要があり、"exam"ユーザーでログインしてください。そうしなければ、リアルモードが実行されません。

## Examshell

`examshell`というプログラムを使用してexamのシステムと連結します。

exam開始から30分以内にこのプログラムをターミナルから起動してください。間違えてプログラムを閉じてしまってもまた`examshell`をターミナルで叩けば再起動します。閉じた場面からまた開始されるだけです。


# Short version

1. examshellを実行する
2. Examshellどのexamにアクセスできるかを出力する
3. どのexamを受けるかとそのexam実行時のレベルを選択
4. システムがGitリポジトリを作成しBASEDIR/renduにクローンする
5. システムがレベルにあった課題をランダムに決めて、アサインする
6. Examshellが課題をBASEDIR/subjects/ASSIGNMENT_NAME/にダウンロードする
7. Examshellがどの課題をやるのか、どれほど得点を獲得できるのか、どこに提出するのかを出力する
8. 課題に取り組む
9. Gitリポジトリに取り組んだ課題をPushする
10. 課題を取り組み終えたと確認した場合、`grademe`コマンドをexamshell上で叩く
11. システムがあなたの取り組んだ課題を評価する
12. BASEDIR/traces/ASSIGNMENT_TRACE_DIRに取り組んだ課題のトレースを保存する
13. 課題を無事クリアした場合、得点を獲得し、レベルが上がる
14. 課題をこなせなかった場合、同じ課題に取り組むことになり、得点を獲得しない

この説明でわからない場合、全部説明しているバージョンを読んでください！

## Step 1: Starting examshell

```
zaz@blackjack ~ $ examshell
examshell v0.1.1

WARNING
Your exam files will be stored in ~/exam-basedir
THIS DIRECTORY WILL BE ENTIRELY EMPTIED BEFORE YOU START
So, if you do have important things there, Ctrl-C NOW and back them up before running this.
(Press Enter to continue...)
```

## Step 2: Preparing the work directory

承認したのち、examshellが環境を整えたり、Gitリポジトリを作成します。

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

Your git repository was successfully cloned to ~/rendu

The following commands are available to you:
  status: Displays the status of your session, including information about
    your current assignment, and the exam history.
  grademe: Asks the server to grade your current assignment. If you
    have done it right, you will gain the points of the current assignment, go
    up a level, and try the next one. If you fail, however, you will stay on the same assignment of the same level to do. So be sure of yourself before you launch
    this command !
  finish: Tells the server you are finished with your exam.

You can log out at any time. If this program tells you you earned points,
then they will be counted whatever happens.

(Press Enter to continue...)
```

## Step 4: Getting an assignment

Examshellがシステムからあなたが取り組むべき課題をフェッチします。
自動で`subjects`ディレクトリに課題が保存されます。ターミナルからアクセスして読み込むことが可能です。

```
[...]
================================================================================
You are currently at level 0
You are running in real mode (Your grade does not count)
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

この表示は`status`コマンドを叩けば閲覧可能です。

## Step 5: Work on your assignment

課題に取り組んでください。

課題は必ずexamshellが指定したディレクトリ（課題と同じ名前のGitリポジトリのサブディレクトリ）に提出しなさい。名前を間違えないでください。間違ったディレクトリで提出してしまった場合、課題をこなせなかったとみなします。後戻りはできません。

たとえば、`max.c`というファイルを`~/exam-basedir/rendu/max/`に入れます。

普段取り組んでいる課題のように、課題に取り組み終えた場合、pushしてください。忘れないで！

## Step 6: Request to be graded

課題に取り組み終えてvogsphereにプッシュしたと確認したのち、`grademe`コマンドを叩けば、提出された課題を評価します。

```
examshell> grademe

Before continuing, please make ABSOLUTELY SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will stay on the same assignment at the same level.

Are you sure? [y/N]
```

まー、本当にこれでいい場合、yを入力しましょう。

```
[...]

Are you sure? [y/N] y
OK, making grading request to server now.

We will now wait for the job to complete.
Please be patient, this CAN take several minutes...
(10 seconds is fast, 30 seconds is expected, 3 minutes is a maximum)
waiting...
```

システムに評価されるのを待ちましょう。時間がかかることがありますが、それは普通の挙動です。焦らず1-2分ほど待ちましょう。
1-2分ほど待っても、何も起きない場合はスタッフを呼んでください。

### Success !

```
[...]
waiting...
>>>>>>>>>> SUCCESS <<<<<<<<<<
You have successfully completed the assignment and earned 16 points!
Trace saved to ~/exam-basedir/traces/0-0_max.trace

(Press Enter to continue...)
```

この場合、成功です。評価の履歴が自動的に保存され、いつでも読み込めます。
次のステータスメッセージは以下の表示をします。

* 得点を獲得しました (16ポイント)
* レベルアップしました (レベル1に上がりました)
* 新しい課題がアサインされました

```
[...]
(Press Enter to continue...)

================================================================================
You are currently at level 1
You are running in real mode (Your grade does not count)
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

examの最終レベルを終えた場合、examshellがexamが終了したと表示します。

### Failure :(

この課題を失敗した場合、次のように表示されます。

```
[...]
examshell> grademe

Before continuing, please make ABSOLUTELY SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will stay on the same assignment at the same level.

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
You are running in real mode (Your grade does not count)
Your current grade is 16/100
Assignments:
  Level 0:
    0: max for 16 potential points (Success)
  Level 1:
    0: wdmatch for 16 potential points (Failure)
    1: wdmatch for 16 potential points (Current)

Your current assignment is wdmatch for 16 potential points
It is assignment 1 for level 1
The subject is located at: ~/exam-basedir/subjects/wdmatch
You must turn in your files in a subdirectory of your Git repository with the
same name as the assignment (~/exam-basedir/rendu/wdmatch).
Of course, you must still push...

The end date for this exam is: 28/05/2015 15:07:47
You have 3 hours, 41 minutes and 25 seconds remaining
================================================================================
You can now work on your assignment. When you are sure you're done with it,
push it to vogsphere, and then use the "grademe" command to be graded.
examshell>
```

この場合、失敗したので

* 得点を獲得しません。
* レベルアップしません。 (レベル1止まりとなる)
* 同じ課題を取り組む

### Error :<

評価の結果、examshellがERRORを表示する可能性があります。これはあなたが課題に失敗したということではなく、システムにエラーが生じたということです。

パニクらないでください。

リトライするか終了（abort）するかのオプションがあります。必ず一回はリトライを選択してください。評価システムのエラーは稀なのでリトライすれば解決されるはずです。
リトライでもこのエラーが解決されない場合はスタッフを呼んでください！

全ての試みが失敗した場合、終了（abort）しましょう。選択した場合、得点は失われず、同じレベルの違う課題が選択されます。

## Step 7: Let's go again

以上です。課題を失敗しすぎて課題が枯渇しない限りか最後のレベルをクリアしていない限り、新しい課題が次々とアサインされます。

`finish`コマンドを叩けば、好きな時にセッションを終えることが可能です。

```
examshell> finish
Please confirm that you REALLY want to end your current session.
If you do, you will not be able to do anything with it anymore!
Are you finished? [y/N] y
Your session has been marked as finished. You may now log out.
zaz@blackjack ~ $
```

# FAQ / 基本的なエラー

## examshellを停止した場合、どうすればいい？

また起動してください。問題ないはずです。

## Examshellが"login window expired"って表示している。どうすればいい？

examセッション開始から30分以内にexamを選択してください。30分以内に選択しない場合、このエラーが表示され、遅すぎたということです。
これに対しては何も行えません。手遅れです。

## Examshellが"Mismatched versions"って表示している。どうすればいい？

あなたが使用しているiMacはアップデートされていないということです。速やかに席を変えましょう。それかボーカルにこのことを伝えてアップデートしてもらいましょう。

## 課題が隣の席に座っている人と同じではなかった！不公平だ！

そうだね。どんまい。

## exam終了後、リポジトリにアクセスできますか？課題を再度読みたい。。。

exam終了後、取り組んだ課題の内容や自分が提出したファイルなどが含まれているメールが届くはずです。

届かなかったら、少し待ってね。本当に届いていない場合、どんまい。人生は不平等だね。

## ExamshellがFAILUREを表示したけど、絶対うまく取り組んだ！

何かし忘れたんじゃないの？

* pushした？
* 本当に？
* 適切なリポジトリで課題に取り組んだの？
* ファイルの名前とかちゃんと合っている？
* 間違った課題を取り組んだ？
* 本当にpushした？
* などなど

本当に本当に本当に全てうまく取り組んだと思った場合は、EXAM終了後、ボーカルに来てください。確認はしますが、その前に：あなたのエラーである可能性は非常に高いです。それとその確認を行うのに時間がかかるかもしれません。

## このシステムダメダメじゃん、古いのに戻して！

そんなわけないじゃん。それと古いのに戻さないよ。

## システムにバグを見つけた！

まじで？

本当に見つけたなら、連絡してね。悪用しなければ怒らないから。

## システムのバグを見つけて悪用したぜ！絶対捕まらない！

いつか、捕まえるからね。違う学校を探し始めた方がいいと思うよ！:)

バカなことしない方がいいよ、本当に。報告するだけでいいから。点数を稼ぎたいがために悪用して退学にはされたくないでしょう？

## 課題を選択するときに時間をかけすぎたとExamshellが表示している。どうすればいい？

何も起こらないよ。再度試してみたら？Examshellはあなたの優柔不断さに呆れているだけだよ。

## 評価リクエスト送ったけど時間がかかっている。。。

1−2分待っても評価が終わらない場合、スタッフを呼んでください。稀なケースです。
