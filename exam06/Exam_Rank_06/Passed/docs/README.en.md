# Introduction

The exam system gives you assignments one by one and will grade you on-demand when you ask it to do it.

An exam is composed of a set of levels. Each level has its pool of possible assignments that are given at random. Your neighbor probably won't have the same as you.

For each level you validate, you'll get points up to 100.

## Before the exam

Before an exam, you subscribe to (or retry, if it isn't the first time) at least one exam project on the Intranet. You can subscribe to as many as you like. There are no restrictions about this.

## Examshell

You will interface with the exam system using a program called `examshell`

You have to start it yourself in your terminal, at the latest 10 minutes after the exam start date. If you stop it inadvertently, it's not a problem. You can start it again by typing `examshell` in a terminal. It will just resume the process to where it was before.

# Short version

1. You start examshell
2. Examshell tells you the different exams you have access to
3. The system creates a Git repository for you and clones it to BASEDIR/rendu
4. The system chooses a random assignment for you to do at the current level
5. Examshell downloads the subject for the assignments to BASEDIR/subjects/ASSIGNMENT_NAME/
6. Examshell tells you what assignment you're supposed to do, how many points you stand to win, and where you should turn it in
7. You work on your assignment
8. You push your assignment on the Git repository as usual
9. When you are sure that you've finished: Tell examshell to grade you using the `grademe` command
10. The system grades your assignment
11. Examshell saves the grading trace to BASEDIR/traces/ASSIGNMENT_TRACE_DIR
12. You will gain points and advance one level if you succeeded
13. You stay at the same assignment if you failed the assignment.

Do you have question? Read the long version!

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


## Step 2: Preparing the working directory

After you confirm, examshell will prepare the working directory, clone the appropriate Git repository, etc...

```
[...]
Confirm? [y/n] y
Selecting project...
Creating required directories...
Ensuring your Git repository for this exam is present and correct...
The Git repository is not cloned yet. Cloning...
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
  finish: Tell the server you are finished with your exam.

You can log out at any time. If this program tells you you earned points,
then they will be counted as whatever happens.

(Press Enter to continue...)
```

## Step 3: Getting an assignment

Examshell will now fetch your current assignment from the system.

It will automatically save the subject in the `subjects` directory. You can read it in a terminal.

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

This display is always accessible using the `status` command.

## Step 4: Work on your assignment

Now, you do the work you are assigned.

Note that you HAVE to turn it in the directory that examshell told you. It is a subdirectory of the Git repository with the same name as the assignment. Careful with the typos. If you use the wrong directory, you will fail the assignment. It will not give you the option to go back.

In our example, we would put our `max.c` file in `~/exam-basedir/rendu/max/`.

You WILL have to push your work after you complete it. It is the same as  any regular project, so don't forget!

## Step 5: Request to be graded

Once you're SURE that your work is correct and that you have pushed it to vogsphere, you can use the `grademe` command to request that your assignment be graded.

```
examshell> grademe

Before continuing, please make SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will stay on the same assignment at the same level.

Are you sure? [y/N]
```

So, yeah, let's say we're sure.

```
[...]

Are you sure? [y/N] y
OK, making a grading request to the server now.

We will now wait for the job to complete.
Please be patient, this CAN take several minutes...
(10 seconds is fast, 30 seconds is expected, 3 minutes is a maximum)
waiting...
```

Now you wait for the system to grade you. It CAN take some time. Don't panic unless you wait for 1-2 minutes. In that case, you should ask a staff member to help you. It's an easy fix.

### Success!

```
[...]
waiting...
>>>>>>>>>> SUCCESS <<<<<<<<<<
You have successfully completed the assignment and earned 16 points!
Trace saved to ~/exam-basedir/traces/0-0_max.trace

(Press Enter to continue...)
```

In this case, you succeeded.

The next status message will show that:

* You earned the points for the assignment
* You have gone up a level
* You have a new assignment

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

If that had been the last level of the exam, examshell would tell you that your exam was finished.

## Failure :(

Let's fail this one to see what it does:

```
[...]
examshell> grademe

Before continuing, please make ABSOLUTELY SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will stay on the same assignment at the same level.

Are you sure? [y/N] y
OK, making a grading request to the server now.

We will now wait for the job to complete.
Please be patient, this CAN take several minutes...
(10 seconds is fast, 30 seconds is expected, and 3 minutes is a maximum)
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

In this case, you failed:

* You do NOT get the points for the  assignment
* You do NOT go up a level
* You keep the same assignment


### Error :<

There is the possibility that examshell says the grading resulted in an ERROR. It does not mean you failed. It means that
the grading system itself has failed.

However, don't panic!

You will have the option to either retry or abort. You SHOULD retry at least once. Errors in the grading system are rare
and usually transient, so they'd resolve with a retry. If a retry does not solve the it, get a staff member to help!

If all else fails, then you can abort. It will give you another assignment of the same level.

## Step 6: Let's go again

You will continue getting new assignments until the exam date ends or you succeed at the last level of the exam.

You can end your session at any time using the `finish` command.

```
examshell> finish
Please confirm that you REALLY want to end your current session.
If you do, you will not be able to do anything with it anymore!
Are you finished? [y/N] y
Your session has been marked as finished. You may now log out.
zaz@blackjack ~ $
```

# FAQ / Common errors

## I stopped examshell. What do I do?

Just start it again. It's no big deal.

## Examshell tells me the "login window expired"

You have 10 minutes to choose an exam after the exam start date. Afterwards, you will get this error because it's simply too late.
No, you can not do anything about it.

## Examshell tells me "Mismatched versions"

The machine you are on is probably just not updated yet. Either switch to another computer, or come to the Bocal and ask us to update it.

## I didn't have the same assignments as my neighbor, it's unfair !

Yes. Too bad. Deal with it.

## Examshell told me I failed, but I'm sure I didn't!

Most likely, you just forgot something :

* Did you push?
* Really?
* Did you use the right directory?
* Are all your files named correctly?
* Did you even do the right assignment?
* Did you REALLY push?
* Etc...

If you are really, really, REALLY, ABSOLUTELY sure you have done it right, you can tell the staff AFTER THE EXAM. We will look into it, but please, please, please think before you come to see us: It is most likely your fault, and it takes us a long time to look for an error in the assignment itself.

## I found a bug in the system!

Really?

Well, if you did, please tell us. We won't get mad at you if you don't exploit it.

## I found a bug in the system, and I exploited it. You will never catch me!

Yes, we will. At some point! And we hope you'll enjoy looking for another school :)

Seriously, do report it and move on. Is it worth getting fired for a few points on an exam?

## I've made a grading request but it is taking a long time

Well, does it finish at some point? Please get a staff member ff it's still not finished after a minute or two. It's not normal, but it's an easy fix for us.
