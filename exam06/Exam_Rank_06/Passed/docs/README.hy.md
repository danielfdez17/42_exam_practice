# Ներածություն

Քննության համակարգում առաջադրանքները տրվելու են մեկ առ մեկ և գնահատվելու եք միայն այն ժամանակ, երբ ինքներդ խնդրեք։

Քննությունն ունի մի քանի մակարդակներ, իսկ յուրաքանչյուր մակարդակ ունի իր հավանական առաջադրանքները, որոնք տրվում են պատահականորեն: Ձեր հարևանը, հավանաբար, չի ունենա նույն առաջադրանքներն, ինչ դուք: Կյանքն անարդար է, հաշտվե՛ք։

Յուրաքանչյուր անցած մակարդակի համար կստանաք միավորներ՝ առավելագույնը 100 միավոր:


## Քննությունից առաջ

Քննությունից առաջ պետք է գրանցվել (կամ կրկին փորձել, եթե առաջին անգամը չէ) ներքնացանցում առնվազն մեկ քննական նախագծի համար: Կարող եք գրանցվել այնքան նախագծերի համար, որքան կցանկանաք․ քանակային սահմանափակումներ չկան:

Գոնե մեկ քննական նախագծի գրանցվելուց հետո այն ձեզ համար հասանելի կլինի «examshell»-ի օգնությամբ:

Եթե ցանկանում եք իսկական քննության մասնակցել, պետք է նաև գրանցվեք քննաշրջանին․ ներկայացե՛ք ԺԱՄԱՆԱԿԻՆ և քննասենյակում մուտք գործե՛ք «exam» մուտքանունով: Միայն դրանից հետո կկարողանաք իրական ռեժիմով անցնել քննությունը:



## Examshell

Քննության համակարգը կիրականացվի «examshell» կոչվող ծրագրով:
Այն պետք է սկսել առանձին տերմինալում, քննության մեկնարկից ոչ ուշ, քան 10 րոպե հետո: Եթե այն պատահաբար դադարեցնեք, խնդիր չէ, կարող եք նորից սկսել՝ տերմինալում մուտքագրելով «examshell»: Պրոցեսը կվերսկսվի ընդհատման պահից։



# Կարճ տարբերակ


1. Գործարկում եք examshell-ը։
2. Examshell-ը ցույց է տալիս, թե ինչ քննություններ են հասանելի։
3. Ընտրում եք քննությունը և մեկնարկային մակարդակը։
4. Համակարգը ձեզ համար ստեղծում է Git պահոց և այն պատճենում է BASEDIR/rendu-ում։
5. Համակարգը ձեզ համար ընտրում է այդ մակարդակի պատահական առաջադրանք։
6. Examshell-ը առաջադրանքների նյութը ներբեռնում է BASEDIR/subjects/ASSIGNMENT_NAME/-ում։
7. Examshell-ը ցույց է տալիս, թե ինչ առաջադրանքներ կան,որտեղ է պետք դրանք հանձնել և քանի միավոր կարող եք ստանալ։ 
8. Աշխատում եք ձեր առաջադրանքի վրա։
9. Ինչպես միշտ, ձեր առաջադրանքը push եք անում Git պահոցի վրա։
10. Երբ վստահ եք, որ ավարտել եք, examshell-ին խնդրում եք գնահատել ձեզ՝ օգտագործելով «grademe» հրամանը։
11. Համակարգը գնահատում է ձեր առաջադրանքը։
12. Examshell-ը ձեր գնահատման պատմությունը պահում է BASEDIR/traces/ASSIGNMENT_TRACE_DIR-ում։
13. Եթե ամեն ինչ հաջող լինի, կստանաք միավորներ և մեկ մակարդակ կբարձրանաք:
14. Հակառակ դեպքում կմնաք նույն առաջադրանքների փուլում և միավորներ չեք ստանա:

Պարզ չէ՞։ Կարդացեք երկար տարբերակը:





## Քայլ 1 examshell-ի գործարկումը 

```
zaz@blackjack ~ $ examshell
examshell v0.1.1

WARNING
Your exam files will be stored in ~/exam-basedir
THIS DIRECTORY WILL BE ENTIRELY EMPTIED BEFORE YOU START
So, if you do have important things there, Ctrl-C NOW and back them up before running this.
(Press Enter to continue...)
```

## Քայլ 2 Աշխատանքային պահոցի պատրաստումը

Ձեր հաստատելուց հետո, examshell-ը կպատրաստի աշխատանքային պահոցը, կպատճենի համապատասխան Git պահոցը, և այլն ...



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

## Քայլ 4 Առաջադրանք ստանալը

Examshell-ը համակարգից այժմ կառանձնացնի ձեր առաջադրանքը:

Ինքնաբերաբար, նյութը կպահվի «subjects» պահոցում: Այն կարող եք կարդալ տերմինալով:




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

Այս հաղորդագրությունը միշտ հասանելի է «status» հրամանով:

## Քայլ 5 Աշխատանք՝ ձեր առաջադրանքի վրա

Այ հիմա սկսում ենք հանձնարարված գործը:

Նկատի ունեցե՛ք, որ առաջադրանքը պետք է հանձնել examshell-ի ցույց տված պահոցում, որը Git շտեմարանի ենթահապահոց է՝ առաջադրանքի անունով: Ոչ մի տառասխալ, կամ այլ բան: Եթե սխալ պահոց օգտագործեք, կձախողեք առաջադրանքը՝ առանց ամեն ինչ վերականգնելու տարբերակի: Տխուր կլինի։

Մեր օրինակում, մեր «max.c» ֆայլը կդնենք «~/exam-basedir/rendu/max/» պանակում:

Աշխատանքը ավարտելուց հետո ՊԵՏՔ Է push անել, ինչպես ցանկացած այլ սովորական նախագծի դեպքում, այնպես որ, չմոռանա՛ք:


## Քայլ 6 Գնահատման խնդրանք

Երբ ՀԱՄՈԶՎԵՔ, որ ձեր աշխատանքը ճիշտ է արված և բոլոր անհրաժեշտ բաները push են արվել, կարող եք օգտագործել «grademe» հրամանը՝ խնդրելու համար, որ ձեր առաջադրանքը գնահատվի։

```
examshell> grademe

Before continuing, please make ABSOLUTELY SURE that you have pushed your files,
that they are in the right directory, that you didn't forget anything, etc...
If your assignment is wrong, you will stay on the same assignment at the same level.

Are you sure? [y/N]
```

Ենթադրենք՝ վստահ ենք:

```
[...]

Are you sure? [y/N] y
OK, making grading request to server now.

We will now wait for the job to complete.
Please be patient, this CAN take several minutes...
(10 seconds is fast, 30 seconds is expected, 3 minutes is a maximum)
waiting...
```

Այժմ սպասում ենք համակարգի գնահատմանը: Իրականում, գնահատումը ԿԱՐՈՂ Է որոշ ժամանակ տևել, դա նորմալ է, այնպես որ խուճապի մի՛ մատնվեք։ Այնուամենայնիվ, եթե արդեն 1-2 րոպե սպասել եք, պարզապես հարցրե՛ք աշխատակազմի անդամին, սա հեշտ լուծվող խնդիր է։


### Հաղթանա՛կ


```
[...]
waiting...
>>>>>>>>>> SUCCESS <<<<<<<<<<
You have successfully completed the assignment and earned 16 points!
Trace saved to ~/exam-basedir/traces/0-0_max.trace

(Press Enter to continue...)
```
Այս դեպքում մեզ մոտ ստացվեց: Գնահատման պատմությունը ինքնաբերաբար պահվում է մեզ համար, եթե ցանկանանք այն տեսնել:

Հաջորդ կարգավիճակի հաղորդագրությունը ցույց կտա, որ.

* առաջադրանքի համար խաղում հավաքեցինք միավորներ՝ (16),
* մի մակարդակ բարձրացել ենք (այժմ 1-ին մակարդակում ենք),
* ունենք նոր առաջադրանք։



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

Եթե դա լիներ քննության վերջին մակարդակը, examshell-ը կասեր դրա մասին, նաև կասեր, որ քննությունն ավարտված է:


### ձախողում :(

Եկեք ձախողենք այս առաջադրանքը՝ պարզապես տեսնելու, թե ինչ է ստացվում.


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
Այս դեպքում մենք ձախողեցինք, այնպես որ.

* միավորներ ՉԵՆՔ ստանում
* ՉԵՆՔ բարձրացնում մակարդակը (մնում ենք 1-ինում)
* Շարունակում ենք նույն առաջադրանքը



### Սխալ :<

Հնարավոր է, examshell-ն ասի, որ գնահատումը հանգեցրել է ERROR-ի: Սա չի նշանակում, որ դուք եք ձախողել, այլ նշանակում է,
որ գնահատման համակարգն ինքն է ձախողվել:

Այնուամենայնիվ, խուճապի մի՛ մատնվեք:

Դուք կունենաք կրկին փորձելու կամ ընդհատելու տարբերակ: ՊԵՏՔ Է գոնե մեկ անգամ կրկին փորձեք, քանի որ գնահատման համակարգում սխալները
հազվադեպ են և, սովորաբար, ժամանակավոր, այնպես որ դրանք կրկին փորձելիս պետք է որ լուծվեն: Եթե կրկին փորձելը չի լուծում սխալը,
դիմե՛ք աշխատակազմին:

Եթե մնացած ամեն ինչը ձախողվի, կարող եք ընդհատել: Ձեզ նույն մակարդակի մեկ այլ առաջադրանք կտրվի, բայց՝ առանց միավորների տուգանքի։


## Քայլ 7 Կրկին անցնենք

Հիմնականում այսքանն է: Կշարունակեք նոր առաջադրանքներ ստանալ այնքան ժամանակ,
քանի դեռ չեք ձախողել բոլոր հնարավոր առաջադրանքներ՝ այդ մակարդակի առաջադրանքների ցանկը սպառելու համար, 
կամ էլ հաջողությամբ ավարտել եք քննության վերջին մակարդակը:

Ցանկության դեպքում, ցանկացած պահի կարող եք ավարտել ձեր քննաշրջանը՝ «finish» հրամանով:


```
examshell> finish
Please confirm that you REALLY want to end your current session.
If you do, you will not be able to do anything with it anymore!
Are you finished? [y/N] y
Your session has been marked as finished. You may now log out.
zaz@blackjack ~ $
```

# ՀՏՀ / Տարածված սխալներ

## Ես դադարեցրել եմ examshell-ը, ի՞նչ անեմ:

Պարզապես նորից սկսե՛ք, մի մեծ խնդիր չէ:

## Examshell-ն ասում է «login window expired»․ ի՞նչ է դա նշանակում:

Քննության մեկնարկից հետո ունեք 10 րոպե, որպեսզի ընտրեք քննությունը, ինչից հետո կստանաք այդ սխալը․ պարզապես ուշացրել եք:
Ո՛չ, այլևս որևէ բան չեք կարող անել:

## Examshell-ն ասում է «Mismatched versions»․ ի՞նչ է դա նշանակում:

Հավանաբար, այն համակարգիչը, որից օգտվում եք, դեռ չի թարմացվել։ Կա՛մ փոխեք համակարգիչը, կա՛մ խնդրեք մեզ թարմացնել այն:

## Իմ առաջադրանքները տարբերվում էին կողքինիս առաջադրանքներից, դա անարդար է:

Այո՛: Շա՛տ վատ է։ Դիմացե՛ք։

## Արդյո՞ք քննությունից հետո հասանելի է իմ պահոցը: Իսկ նյութերը՞:

Սովորաբար, քննության ավարտից հետո այդ ամենի մասին էլեկտրոնային նամակ կստանաք:

Եթե չեք ստացել, սպասե՛ք: Եթե արդեն երկար եք սպասել ու միևնույն է չեք ստացել նամակը, ինչ ասենք․ կյանքն անարդար է:

## Examshell-ն ասում է՝ ձախողել եմ, բայց համոզված եմ, որ այդպես չէ:

Ամենայն հավանականությամբ, պարզապես ինչ-որ բան մոռացել եք՝

* push արե՞լ եք:
* Իսկապե՞ս։
* Ճի՞շտ պահոցն եք օգտագործել:
* Ձեր բոլոր ֆայլերը ճի՞շտ անուն ունեն:
* Հաստա՞տ ճիշտ առաջադրանքն եք կատարել:
* ԻՐՈ՞Ք push արել եք:
* Եվ այլն ...

Եթե դուք իսկապես, իսկապես, ԻՍԿԱՊԵՍ վստահ եք, որ ամեն ինչ ճիշտ եք արել, ապա քննության ԱՎԱՐՏԻՑ ՀԵՏՈ պարզապես ասե՛ք աշխատակիցներին: 
Մենք կանդրադառնանք դրան, բայց խնդրում ենք, խնդրում ենք, մտածե՛ք նախքան մեզ դիմելը։ 
Ամենայն հավանականությամբ դա ձեր մեղքն է, և մեզանից երկար ժամանակ կպահանջվի ձեր առաջադրանքում սխալ գտնելու համար։

## Ձեր համակարգը շատ վատն է, ես հինն եմ ուզում:

Ո՛չ, դա այդպես չէ և ո՛չ, չե՛ք կարող վերադարձնել հինը:

## Համակարգում սխալ եմ գտել:

* Իսկապե՞ս։

Դե, եթե դա գտել եք, խնդրում ենք՝ մեզ ասել: Մենք չենք բարկանա ձեզ վրա, եթե սխալից չփորձեք օգտվել։ 

## Ես համակարգում սխալ եմ գտել և այն շահագործել եմ, իսկ դուք ինձ երբեք չե՛ք բռնի:

Այո՛, մենք կբռնե՛նք: Ինչ-որ պահի: Եվ հուսով ենք, որ կվայելեք մեկ այլ դպրոց փնտրելը :)

Լուրջ, ապուշ մի՛ եղեք, պարզապես զեկուցե՛ք և առաջ շարժվե՛ք: Քննության մի քանի միավորների համար չարժի հեռացվել, այդպես չէ՞:

## Examshell-ն ասում է, որ նախագիծ ընտրելու համար ես շատ երկար եմ սպասել , ի՞նչ է դա նշանակում:

Ոչինչ սխալ չէ, պարզապես նորից փորձե՛ք: Դա իրականում սխալ չէ․ պարզապես examshell-ի անշնորհք ձևն է ձեզ ասելու, որ չափազանց անվճռական եք:

## Ես գնահատում եմ պահանջել, բայց դա շատ երկար է տևում։

Իսկ այն ինչ-որ պահի ավարտվո՞ւմ է: Եթե մեկ կամ երկու րոպե անց այն դեռ չի ավարտվել,
պարզապես դիմե՛ք աշխատակազմին, քանի որ սա արդեն նորմալ վարք չէ։
Սովորաբար, այնուամենայնիվ, սա հեշտ լուծվող խնդիր է:





