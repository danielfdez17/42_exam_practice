# 소개

시험 시스템은 과제를 하나씩 제공하며 채점은 채점을 요청할 시에만 진행됩니다.

시험은 여러 레벨로 이루어져있으며 각 레벨에 있는 많은 과제들 중 무작위로 출제됩니다. 같은 레벨이어도 다른 응시자는 다른 문제를 받을 수도 있습니다. 인생은 원래 불공평합니다. 받아들이세요.

각 레벨을 성공할때마다 포인트를 얻으며, 최대 100 점까지 받을 수 있습니다.

## 시험 전에

시험을 시작하기 전에 인트라넷에서 최소 한개 이상의 시험 프로젝트에 등록(처음 시도가 아닌 경우, 재시도) 해야 합니다. 원하는만큼 등록 할 수 있으며 이에 대한 제한은 없습니다.

적어도 하나의 시험 프로젝트를 등록한 경우 `examshell '을 사용하여 액세스 할 수 있습니다.

시험을 실제로 진행하려면 시험 세션에 등록하고 시간에 맞추어 시험장소에 도착하여  "exam"으로 로그인해야합니다. 그래야만 real mode로 시험을 볼 수 있습니다.

## Examshell

"examshell"이라는 프로그램을 사용하여 시험 시스템에 접속해야합니다.

시험이 시작되고 10 분 안에 터미널에서 직접 실행해야합니다. 중간에 실수로 나가더라도 문제가 되지 않으니 터미널에 "examshell"을 입력하여 다시 시작할 수 있습니다. 이 경우, 프로세스가 접속이 끊긴 시점에서 다시 시작됩니다.

# 짧은 버전

1. Examshell을 시작하십시오.
2. Examshell에 접속할수있는 시험목록이 표시될 것입니다.
3. 시작 할 레벨을 선택하십시오.
4. 시스템이 Git 저장소를 생성하고 BASEDIR/rendu에 클론이 있습니다.
5. 시스템이 현재 레벨에서 무작위로 과제를 선택합니다.
6. Examshell은 BASEDIR/subjects/ASSIGNMENT_NAME/에 과제 대상을 다운로드합니다.
7. Examshell은 해야 할 과제, 얻을 수있는 포인트, 그리고 어디에 과제를 제출해야하는지 알려줍니다.
8. 주어진 과제를 하십시오.
9. 평상시처럼 Git 저장소에서 과제를  push합니다.
10. 과제를 완료했다면: `grademe` 명령을 사용하여 examshell에게 채점을 하게 합니다.
11. 시스템이 과제를 채점할 것입니다.
12. Examshell은 정지 추적을 BASEDIR / traces / ASSIGNMENT_TRACE_DIR에 저장합니다.
13. 성공하면 포인트를 얻고 한 단계 위로 올라갑니다.
14. 실패하면, 아무 포인트를 얻지 못하고 재시도 하여야합니다.

이해하지 안가시나요? 긴 버전을 읽어보십시오!

## 1단계: examshell 시작하기

```
zaz@blackjack ~ $ examshell
examshell v0.1.1

WARNING
Your exam files will be stored in ~/exam-basedir
THIS DIRECTORY WILL BE ENTIRELY EMPTIED BEFORE YOU START
So, if you do have important things there, Ctrl-C NOW and back them up before running this.
(Press Enter to continue...)
```
## 2단계: 작업 디렉토리 준비하기
확인 후 examshell은 작업 디렉토리를 준비하고 적절한 Git 저장소 등을 클론합니다.

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

## 4단계: 과제 받기

Examshell은 이제 시스템에서 현재 과제를 가져옵니다.

과제 문제를 `subjects` 디렉토리에 자동 저장합니다. 터미널에서 읽을 수 있습니다.

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

위의  디스플레이는 `status` 명령을 사용하여 항상 액세스 할 수 있습니다.

## 5단계: 과제 풀기
자, 이제 받은 과제를 풉니다.

examshell이 만들라고 정해준 Git 저장소의 서브 디렉토리 인 디렉토리에 과제를 제출합니다. 디렉토리명은 과제가 명시한 것과 동일해야 합니다. 오타를 포함한 그 어떠한 실수도 용납되지 않습니다. 잘못된 디렉토리를 사용하면, 아쉽지만 과제는 실패하게 되고 되돌릴 수 없습니다. 

지금의 예에서는`max.c` 파일을 `~/exam-basedir/rendu/max/`에 넣습니다.

일반 프로젝트와 마찬가지로 작업을 마친 후에 과제를 push해야합니다. 잊지마세요!

## 6 단계 : 채점 요청하기

작업이 올바르게 완료되었고 vogsphere로 push한 것이 확실하다면 `grademe` 명령어을 사용하여 과제의 채점을 요청할 수 있습니다.

```
examshell> grademe
Before continuing, please make ABSOLUTELY SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will stay on the same assignment at the same level.

Are you sure? [y/N]
```
좋습니다, 우리가 확실하다고 가정 해 봅시다.

```
[...]

Are you sure? [y/N] y
OK, making grading request to server now.

We will now wait for the job to complete.
Please be patient, this CAN take several minutes...
(10 seconds is fast, 30 seconds is expected, 3 minutes is a maximum)
waiting...
```
이제 시스템이 평가할 때까지 기다립니다. 실제로는 약간의 시간이 걸릴 수 있으니, 1-2 분 이상 기다려도 끝나지 않는 한 당황하지 마십시오. 기다림이 계속 되는  경우, 스태프에게 물어 보면 간단히 고칠 수 있습니다.

### 성공!
```
[...]
waiting...
>>>>>>>>>> SUCCESS <<<<<<<<<<
You have successfully completed the assignment and earned 16 points!
Trace saved to ~/exam-basedir/traces/0-0_max.trace

(Press Enter to continue...)
```
과제에 성공했습니다. 원할 경우 읽을 수 있도록 채점 결과가 자동으로 저장됩니다.

다음 상태 메시지는 다음을 보여줍니다:
* We earned the points in play for the assignment (16)
* We have gone up a level (now at 1)
* We have a new assignment

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
만약 현 상태가 시험의 마지막 레벨이라면, examshell은 시험이 완료되었다고 말할것입니다.

### 실패:(

과제 실패 시에 어떻게 되는지 보기 위해 실패해봅시다 :
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
실패한 경우입니다. 그로므로...

* 포인트를 얻지 못합니다
* 레벨이  올라가지 않습니다 (1에 머물러 있습니다)
* 같은 과제를 유지합니다.

### 오류 : <

examshell에서 채점 결과 오류가 발생했을 가능성이 있습니다. 그렇다고 실패했다는 의미는 아닙니다.
채점 시스템 자체가 실패했다는 말입니다.

당황하지 마십시오!

과제를 재시도하거나 중단 할 수 있습니다. 채점 시스템의 오류는 드물기 때문에 한 번 이상 다시 다시 채점을 시도해 보십시오.
일반적으로 시스템의 오류는 일시적이므로 재시도로 해결됩니다. 다시 시도해도 오류가 해결되지 않으면 직원에게 도움을 요청하십시오!

다른 모든 방법이 실패하면 중단 할 수 있습니다. 이것은 당신에게 같은 수준의 다른 과제를 줄 것이고 해당 단계의 잠재적인 점수를 잃지 않습니다.

## 7 단계 : 다시 해봅시다.

기본적으로 그렇습니다. 수준의 과제 풀을 모두 소진할때까지 또는 시험의 마지막 단계에서 성공할 때까지 새로운 과제를 계속 받게됩니다.

원하는 경우`finish` 명령을 사용하여 언제든지 세션을 종료 할 수 있습니다.
```
examshell> finish
Please confirm that you REALLY want to end your current session.
If you do, you will not be able to do anything with it anymore!
Are you finished? [y/N] y
Your session has been marked as finished. You may now log out.
zaz@blackjack ~ $
```

# FAQ / 자주묻는 질문

## examshell을 나가버렸습니다. 어떻게해야합니까?

문제없습니다. 다시 시작하면 됩니다.

## Examshell에 "login window expired"라는 메세지가 뜹니다.

시험 시작 후 10 분안에 시험을 선택하지 않으면 오류가 발생합니다.
더 이상 할 수 있는 것은 없습니다. 축하합니다. 그대로 퇴실하시면 됩니다.

## Examshell은 "일치하지 않는 버전"을 알려줍니다.

사용중인 컴퓨터가 아직 업데이트되지 않았을 수 있습니다. 다른 컴퓨터로 전환하거나 Bocal로 이동하여 업데이트를 요청하십시오.

## 이웃과 같은 과제가 없었습니다. 불공평합니다!

예. 아쉽지만  받아 들여야합니다.

## 시험 후 저장소에 액세스 할 수 있습니까? subjects는요?

일반적으로 시험이 완료된 것으로 처리가되면 시험의  모든 내용이 이메일로 발송됩니다.
만약 메일이 오지 않는다다면, 기다리십시오. 당신이 정말로 그것을받지 못하면 아쉽지만 인생은 불공평합니다.

## Examshell이 실패했다고 말했지만, 전 절대 틀리지 않았다고 확신합니다!

아마도, 당신은 무언가를 깜빡했습니다.

* push 하셨습니까?
* 정말로 ?
* 올바른 디렉토리를 사용 했습니까?
* 모든 파일의 이름이 정확합니까?
* 올바른 과제를 했습니까?
* 정말로 push하셨습니까?
* 기타 ...

당신이 정말로, 정말로, 정말로, 절대적으로, 당신이 올바르게했는지 확실하다면, 시험 후 직원에게 알려주십시오. 우리는 그것을 볼 것이지만, 제발, 제발, 제발, 당신이 우리를 만나기 전에 생각하십시오 : 그것은 실제로 당신의 잘못 일 가능성이 있으며 과제 자체에서 오류를 찾기 위해 오랜 시간이 걸립니다.

## 당신의 시스템은 짜증나고, 나는 이전 버전을 원해!

이 시스템은 그정도로 짜증나지는 않습니다.  그리고 이전 버전을 줄 수 없습니다.

## 시스템에서 버그를 발견했습니다!

정말 ?

우리에게 알려주십시오. 당신이 그것을 이용하지 않으면 우리는 당신에게 화 내지 않을 것입니다.

## 시스템에서 버그를 발견했고 그것을 악용했습니다. 그리고 당신들은 저를 잡을 수 없을 것입니다!

찾을 수 있습니다. 언젠가는! 그리고 우리는 당신이 다른 학교를 찾는 것을 즐기기를 바랍니다 :)

진지하게, 바보가되지 말고 보고하고 넘어가십시오. 점수 몇 포인트때문에 퇴학 당할 가치가 없지않나요?

## Examshell은 프로젝트를 선택하기에 너무 오래 기다렸다 고 말합니다.

문제가 아닙니다. 다시 시도해 보십시오. 그것은 실제로 오류가 아니며, 당신이 선택장애가 있다는 것을 알려주려는 examshell의 서투른 메세지입니다.

## 채점 요청을했지만 시간이 오래 걸립니다

글쎄, 그것은 실제로 어느 시점에서 끝나나요? 1 ~ 2 분 후에도 여전히 끝나지 않으면 스태프에게 문의하십시오. 일반적인 상황은 아니지만 보통 쉽게 해결됩니다.

