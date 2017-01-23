/*
  Sentiment analysis
  Copyright (C) 2017 Bob Mottram
  bob@freedombone.net

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gnu-slash-mood.h"

/* Note: All words should be in lower case */

/* default trigger words */

char * trigger_word_en[] = {
    "gnusocial", "mastodon", "stallman", "postactiv", "signal", "linux", "libreboot", "#nsfw",
    NULL
};

char * egocentrism_word_en[] = {
    "i", "me", "my", "myself", "mine", "i'll", "i'd", "i'm",
    "they", "he", "she", "their", "them", "people", "users", "our", "us", "you", "your", "people's", "we", "we're", "they're", "you're", "he's", "she's", "they'll", "peoples",
    NULL
};

static int egocentrism_value[] = {
    2, 2, 2, 2, 2, 2, 2, 2,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};


char * gender_word_en[] = {
    "she", "her", "woman", "female", "feminine",
    "fem", "mother", "sister", "daughter", "girlfriend",
    "wife", "aunt", "mrs", "miss", "girl",
    "girls", "ms", "women", "mothers", "sisters",
    "women's", "womens", "mother's", "sister's", "lady",
    "ladies", "lady's",
    "he", "his", "him", "man", "male",
    "masculine", "butch", "macho", "father", "brother",
    "husband", "uncle", "son", "mr", "boy",
    "boys", "men", "husbands", "fathers", "brothers",
    "mens", "men's", "husband's", "father's", "brother's",
    "sir",
    NULL
};

static int gender_value[] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1
};

/*
  Words from AFINN-111 under Open Database License (ODbL) v1.0
  http://www2.imm.dtu.dk/pubdb/views/publication_details.php?id=6010
  http://www.opendatacommons.org/licenses/odbl/1.0/
*/

char * sentiment_word_en[] = {
    "abandon","abandoned","abandons","abducted","abduction","abductions","abhor","abhorred","abhorrent","abhors","abilities","ability","aboard","absentee","absentees","absolve","absolved","absolves","absolving","absorbed","abuse","abused","abuses","abusive","accept","accepted","accepting","accepts","accident","accidental","accidentally","accidents","accomplish","accomplished","accomplishes","accusation","accusations","accuse","accused","accuses","accusing","ache","achievable","aching","acquit","acquits","acquitted","acquitting","acrimonious","active","adequate","admire","admired","admires","admiring","admit","admits","admitted","admonish","admonished","adopt","adopts","adorable","adore","adored","adores","advanced","advantage","advantages","adventure","adventures","adventurous","affected","affection","affectionate","afflicted","affronted","afraid","aggravate","aggravated","aggravates","aggravating","aggression","aggressions","aggressive","aghast","agog","agonise","agonised","agonises","agonising","agonize","agonized","agonizes","agonizing","agree","agreeable","agreed","agreement","agrees","alarm","alarmed","alarmist","alarmists","alas","alert","alienation","alive","allergic","allow","alone","amaze","amazed","amazes","amazing","ambitious","ambivalent","amuse","amused","amusement","amusements","anger","angers","angry","anguish","anguished","animosity","annoy","annoyance","annoyed","annoying","annoys","antagonistic","anti","anticipation","anxiety","anxious","apathetic","apathy","apeshit","apocalyptic","apologise","apologised","apologises","apologising","apologize","apologized","apologizes","apologizing","apology","appalled","appalling","appease","appeased","appeases","appeasing","applaud","applauded","applauding","applauds","applause","appreciate","appreciated","appreciates","appreciating","appreciation","apprehensive","approval","approved","approves","ardent","arrest","arrested","arrests","arrogant","ashame","ashamed","ass","assassination","assassinations","asset","assets","assfucking","asshole","astonished","astound","astounded","astounding","astoundingly","astounds","attack","attacked","attacking","attacks","attract","attracted","attracting","attraction","attractions","attracts","audacious","authority","avert","averted","averts","avid","avoid","avoided","avoids","await","awaited","awaits","award","awarded","awards","awesome","awful","awkward","axe","axed","backed","backing","backs","bad","badass","badly","bailout","bamboozle","bamboozled","bamboozles","ban","banish","bankrupt","bankster","banned","bargain","barrier","bastard","bastards","battle","battles","beaten","beatific","beating","beauties","beautiful","beautifully","beautify","belittle","belittled","beloved","benefit","benefits","benefitted","benefitting","bereave","bereaved","bereaves","bereaving","best","betray","betrayal","betrayed","betraying","betrays","better","bias","biased","big","bitch","bitches","bitter","bitterly","bizarre","blah","blame","blamed","blames","blaming","bless","blesses","blessing","blind","bliss","blissful","blithe","block","blockbuster","blocked","blocking","blocks","bloody","blurry","boastful","bold","boldly","bomb","boost","boosted","boosting","boosts","bore","bored","boring","bother","bothered","bothers","bothersome","boycott","boycotted","boycotting","boycotts","brainwashing","brave","breakthrough","breathtaking","bribe","bright","brightest","brightness","brilliant","brisk","broke","broken","brooding","bullied","bullshit","bully","bullying","bummer","buoyant","burden","burdened","burdening","burdens","calm","calmed","calming","calms","can't stand","cancel","cancelled","cancelling","cancels","cancer","capable","captivated","care","carefree","careful","carefully","careless","cares","cashing in","casualty","catastrophe","catastrophic","cautious","celebrate","celebrated","celebrates","celebrating","censor","censored","censors","certain","chagrin","chagrined","challenge","chance","chances","chaos","chaotic","charged","charges","charm","charming","charmless","chastise","chastised","chastises","chastising","cheat","cheated","cheater","cheaters","cheats","cheer","cheered","cheerful","cheering","cheerless","cheers","cheery","cherish","cherished","cherishes","cherishing","chic","childish","chilling","choke","choked","chokes","choking","clarifies","clarity","clash","classy","clean","cleaner","clear","cleared","clearly","clears","clever","clouded","clueless","cock","cocksucker","cocksuckers","cocky","coerced","collapse","collapsed","collapses","collapsing","collide","collides","colliding","collision","collisions","colluding","combat","combats","comedy","comfort","comfortable","comforting","comforts","commend","commended","commit","commitment","commits","committed","committing","compassionate","compelled","competent","competitive","complacent","complain","complained","complains","comprehensive","conciliate","conciliated","conciliates","conciliating","condemn","condemnation","condemned","condemns","confidence","confident","conflict","conflicting","conflictive","conflicts","confuse","confused","confusing","congrats","congratulate","congratulation","congratulations","consent","consents","consolable","conspiracy","constrained","contagion","contagions","contagious","contempt","contemptuous","contemptuously","contend","contender","contending","contentious","contestable","controversial","controversially","convince","convinced","convinces","convivial","cool","cool stuff","cornered","corpse","costly","courage","courageous","courteous","courtesy","cover-up","coward","cowardly","coziness","cramp","crap","crash","crazier","craziest","crazy","creative","crestfallen","cried","cries","crime","criminal","criminals","crisis","critic","criticism","criticize","criticized","criticizes","criticizing","critics","cruel","cruelty","crush","crushed","crushes","crushing","cry","crying","cunt","curious","curse","cut","cute","cuts","cutting","cynic","cynical","cynicism","damage","damages","damn","damned","damnit","danger","daredevil","daring","darkest","darkness","dauntless","dead","deadlock","deafening","dear","dearly","death","debonair","debt","deceit","deceitful","deceive","deceived","deceives","deceiving","deception","decisive","dedicated","defeated","defect","defects","defender","defenders","defenseless","defer","deferring","defiant","deficit","degrade","degraded","degrades","dehumanize","dehumanized","dehumanizes","dehumanizing","deject","dejected","dejecting","dejects","delay","delayed","delight","delighted","delighting","delights","demand","demanded","demanding","demands","demonstration","demoralized","denied","denier","deniers","denies","denounce","denounces","deny","denying","depressed","depressing","derail","derailed","derails","deride","derided","derides","deriding","derision","desirable","desire","desired","desirous","despair","despairing","despairs","desperate","desperately","despondent","destroy","destroyed","destroying","destroys","destruction","destructive","detached","detain","detained","detention","determined","devastate","devastated","devastating","devoted","diamond","dick","dickhead","die","died","difficult","diffident","dilemma","dipshit","dire","direful","dirt","dirtier","dirtiest","dirty","disabling","disadvantage","disadvantaged","disappear","disappeared","disappears","disappoint","disappointed","disappointing","disappointment","disappointments","disappoints","disaster","disasters","disastrous","disbelieve","discard","discarded","discarding","discards","disconsolate","disconsolation","discontented","discord","discounted","discouraged","discredited","disdain","disgrace","disgraced","disguise","disguised","disguises","disguising","disgust","disgusted","disgusting","disheartened","dishonest","disillusioned","disinclined","disjointed","dislike","dismal","dismayed","disorder","disorganized","disoriented","disparage","disparaged","disparages","disparaging","displeased","dispute","disputed","disputes","disputing","disqualified","disquiet","disregard","disregarded","disregarding","disregards","disrespect","disrespected","disruption","disruptions","disruptive","dissatisfied","distort","distorted","distorting","distorts","distract","distracted","distraction","distracts","distress","distressed","distresses","distressing","distrust","distrustful","disturb","disturbed","disturbing","disturbs","dithering","dizzy","dodging","dodgy","does not work","dolorous","dont like","doom","doomed","doubt","doubted","doubtful","doubting","doubts","douche","douchebag","downcast","downhearted","downside","drag","dragged","drags","drained","dread","dreaded","dreadful","dreading","dream","dreams","dreary","droopy","drop","drown","drowned","drowns","drunk","dubious","dud","dull","dumb","dumbass","dump","dumped","dumps","dupe","duped","dysfunction","eager","earnest","ease","easy","ecstatic","eerie","eery","effective","effectively","elated","elation","elegant","elegantly","embarrass","embarrassed","embarrasses","embarrassing","embarrassment","embittered","embrace","emergency","empathetic","emptiness","empty","enchanted","encourage","encouraged","encouragement","encourages","endorse","endorsed","endorsement","endorses","enemies","enemy","energetic","engage","engages","engrossed","enjoy","enjoying","enjoys","enlighten","enlightened","enlightening","enlightens","ennui","enrage","enraged","enrages","enraging","enrapture","enslave","enslaved","enslaves","ensure","ensuring","enterprising","entertaining","enthral","enthusiastic","entitled","entrusted","envies","envious","envy","envying","erroneous","error","errors","escape","escapes","escaping","esteemed","ethical","euphoria","euphoric","eviction","evil","exaggerate","exaggerated","exaggerates","exaggerating","exasperated","excellence","excellent","excite","excited","excitement","exciting","exclude","excluded","exclusion","exclusive","excuse","exempt","exhausted","exhilarated","exhilarates","exhilarating","exonerate","exonerated","exonerates","exonerating","expand","expands","expel","expelled","expelling","expels","exploit","exploited","exploiting","exploits","exploration","explorations","expose","exposed","exposes","exposing","extend","extends","exuberant","exultant","exultantly","fabulous","fad","fag","faggot","faggots","fail","failed","failing","fails","failure","failures","fainthearted","fair","faith","faithful","fake","fakes","faking","fallen","falling","falsified","falsify","fame","fan","fantastic","farce","fascinate","fascinated","fascinates","fascinating","fascist","fascists","fatalities","fatality","fatigue","fatigued","fatigues","fatiguing","favor","favored","favorite","favorited","favorites","favors","fear","fearful","fearing","fearless","fearsome","fed up","feeble","feeling","felonies","felony","fervent","fervid","festive","fiasco","fidgety","fight","fine","fire","fired","firing","fit","fitness","flagship","flees","flop","flops","flu","flustered","focused","fond","fondness","fool","foolish","fools","forced","foreclosure","foreclosures","forget","forgetful","forgive","forgiving","forgotten","fortunate","frantic","fraud","frauds","fraudster","fraudsters","fraudulence","fraudulent","free","freedom","frenzy","fresh","friendly","fright","frightened","frightening","frikin","frisky","frowning","frustrate","frustrated","frustrates","frustrating","frustration","ftw","fuck","fucked","fucker","fuckers","fuckface","fuckhead","fucking","fucktard","fud","fuked","fuking","fulfill","fulfilled","fulfills","fuming","fun","funeral","funerals","funky","funnier","funny","furious","futile","gag","gagged","gain","gained","gaining","gains","gallant","gallantly","gallantry","generous","genial","ghost","giddy","gift","glad","glamorous","glamourous","glee","gleeful","gloom","gloomy","glorious","glory","glum","god","goddamn","godsend","good","goodness","grace","gracious","grand","grant","granted","granting","grants","grateful","gratification","grave","gray","great","greater","greatest","greed","greedy","green wash","green washing","greenwash","greenwasher","greenwashers","greenwashing","greet","greeted","greeting","greetings","greets","grey","grief","grieved","gross","growing","growth","guarantee","guilt","guilty","gullibility","gullible","gun","ha","hacked","haha","hahaha","hahahah","hail","hailed","hapless","haplessness","happiness","happy","hard","hardier","hardship","hardy","harm","harmed","harmful","harming","harms","harried","harsh","harsher","harshest","hate","hated","haters","hates","hating","haunt","haunted","haunting","haunts","havoc","healthy","heartbreaking","heartbroken","heartfelt","heaven","heavenly","heavyhearted","hell","help","helpful","helping","helpless","helps","hero","heroes","heroic","hesitant","hesitate","hid","hide","hides","hiding","highlight","hilarious","hindrance","hoax","homesick","honest","honor","honored","honoring","honour","honoured","honouring","hooligan","hooliganism","hooligans","hope","hopeful","hopefully","hopeless","hopelessness","hopes","hoping","horrendous","horrible","horrific","horrified","hostile","huckster","hug","huge","hugs","humerous","humiliated","humiliation","humor","humorous","humour","humourous","hunger","hurrah","hurt","hurting","hurts","hypocritical","hysteria","hysterical","hysterics","idiot","idiotic","ignorance","ignorant","ignore","ignored","ignores","ill","illegal","illiteracy","illness","illnesses","imbecile","immobilized","immortal","immune","impatient","imperfect","importance","important","impose","imposed","imposes","imposing","impotent","impress","impressed","impresses","impressive","imprisoned","improve","improved","improvement","improves","improving","inability","inaction","inadequate","incapable","incapacitated","incensed","incompetence","incompetent","inconsiderate","inconvenience","inconvenient","increase","increased","indecisive","indestructible","indifference","indifferent","indignant","indignation","indoctrinate","indoctrinated","indoctrinates","indoctrinating","ineffective","ineffectively","infatuated","infatuation","infected","inferior","inflamed","influential","infringement","infuriate","infuriated","infuriates","infuriating","inhibit","injured","injury","injustice","innovate","innovates","innovation","innovative","inquisition","inquisitive","insane","insanity","insecure","insensitive","insensitivity","insignificant","insipid","inspiration","inspirational","inspire","inspired","inspires","inspiring","insult","insulted","insulting","insults","intact","integrity","intelligent","intense","interest","interested","interesting","interests","interrogated","interrupt","interrupted","interrupting","interruption","interrupts","intimidate","intimidated","intimidates","intimidating","intimidation","intricate","intrigues","invincible","invite","inviting","invulnerable","irate","ironic","irony","irrational","irresistible","irresolute","irresponsible","irreversible","irritate","irritated","irritating","isolated","itchy","jackass","jackasses","jailed","jaunty","jealous","jeopardy","jerk","jesus","jewel","jewels","jocular","join","joke","jokes","jolly","jovial","joy","joyful","joyfully","joyless","joyous","jubilant","jumpy","justice","justifiably","justified","keen","kill","killed","killing","kills","kind","kinder","kiss","kudos","lack","lackadaisical","lag","lagged","lagging","lags","lame","landmark","laugh","laughed","laughing","laughs","laughting","launched","lawl","lawsuit","lawsuits","lazy","leak","leaked","leave","legal","legally","lenient","lethargic","lethargy","liar","liars","libelous","lied","lifesaver","lighthearted","like","liked","likes","limitation","limited","limits","litigation","litigious","lively","livid","lmao","lmfao","loathe","loathed","loathes","loathing","lobby","lobbying","lol","lonely","lonesome","longing","loom","loomed","looming","looms","loose","looses","loser","losing","loss","lost","lovable","love","loved","lovelies","lovely","loving","lowest","loyal","loyalty","luck","luckily","lucky","lugubrious","lunatic","lunatics","lurk","lurking","lurks","mad","maddening","made-up","madly","madness","mandatory","manipulated","manipulating","manipulation","marvel","marvelous","marvels","masterpiece","masterpieces","matter","matters","mature","meaningful","meaningless","medal","mediocrity","meditative","melancholy","menace","menaced","mercy","merry","mess","messed","messing up","methodical","mindless","miracle","mirth","mirthful","mirthfully","misbehave","misbehaved","misbehaves","misbehaving","mischief","mischiefs","miserable","misery","misgiving","misinformation","misinformed","misinterpreted","misleading","misread","misreporting","misrepresentation","miss","missed","missing","mistake","mistaken","mistakes","mistaking","misunderstand","misunderstanding","misunderstands","misunderstood","moan","moaned","moaning","moans","mock","mocked","mocking","mocks","mongering","monopolize","monopolized","monopolizes","monopolizing","moody","mope","moping","moron","motherfucker","motherfucking","motivate","motivated","motivating","motivation","mourn","mourned","mournful","mourning","mourns","mumpish","murder","murderer","murdering","murderous","murders","myth","n00b","naive","nasty","natural","naïve","needy","negative","negativity","neglect","neglected","neglecting","neglects","nerves","nervous","nervously","nice","nifty","niggas","nigger","no","no fun","noble","noisy","nonsense","noob","nosey","not good","not working","notorious","novel","numb","nuts","obliterate","obliterated","obnoxious","obscene","obsessed","obsolete","obstacle","obstacles","obstinate","odd","offend","offended","offender","offending","offends","offline","oks","ominous","once-in-a-lifetime","opportunities","opportunity","oppressed","oppressive","optimism","optimistic","optionless","outcry","outmaneuvered","outrage","outraged","outreach","outstanding","overjoyed","overload","overlooked","overreact","overreacted","overreaction","overreacts","oversell","overselling","oversells","oversimplification","oversimplified","oversimplifies","oversimplify","overstatement","overstatements","overweight","oxymoron","pain","pained","panic","panicked","panics","paradise","paradox","pardon","pardoned","pardoning","pardons","parley","passionate","passive","passively","pathetic","pay","peace","peaceful","peacefully","penalty","pensive","perfect","perfected","perfectly","perfects","peril","perjury","perpetrator","perpetrators","perplexed","persecute","persecuted","persecutes","persecuting","perturbed","pesky","pessimism","pessimistic","petrified","phobic","picturesque","pileup","pique","piqued","piss","pissed","pissing","piteous","pitied","pity","playful","pleasant","please","pleased","pleasure","poised","poison","poisoned","poisons","pollute","polluted","polluter","polluters","pollutes","poor","poorer","poorest","popular","positive","positively","possessive","postpone","postponed","postpones","postponing","poverty","powerful","powerless","praise","praised","praises","praising","pray","praying","prays","prblm","prblms","prepared","pressure","pressured","pretend","pretending","pretends","pretty","prevent","prevented","preventing","prevents","prick","prison","prisoner","prisoners","privileged","proactive","problem","problems","profiteer","progress","prominent","promise","promised","promises","promote","promoted","promotes","promoting","propaganda","prosecute","prosecuted","prosecutes","prosecution","prospect","prospects","prosperous","protect","protected","protects","protest","protesters","protesting","protests","proud","proudly","provoke","provoked","provokes","provoking","pseudoscience","punish","punished","punishes","punitive","pushy","puzzled","quaking","questionable","questioned","questioning","racism","racist","racists","rage","rageful","rainy","rant","ranter","ranters","rants","rape","rapist","rapture","raptured","raptures","rapturous","rash","ratified","reach","reached","reaches","reaching","reassure","reassured","reassures","reassuring","rebellion","recession","reckless","recommend","recommended","recommends","redeemed","refuse","refused","refusing","regret","regretful","regrets","regretted","regretting","reject","rejected","rejecting","rejects","rejoice","rejoiced","rejoices","rejoicing","relaxed","relentless","reliant","relieve","relieved","relieves","relieving","relishing","remarkable","remorse","repulse","repulsed","rescue","rescued","rescues","resentful","resign","resigned","resigning","resigns","resolute","resolve","resolved","resolves","resolving","respected","responsible","responsive","restful","restless","restore","restored","restores","restoring","restrict","restricted","restricting","restriction","restricts","retained","retard","retarded","retreat","revenge","revengeful","revered","revive","revives","reward","rewarded","rewarding","rewards","rich","ridiculous","rig","rigged","right direction","rigorous","rigorously","riot","riots","risk","risks","rob","robber","robed","robing","robs","robust","rofl","roflcopter","roflmao","romance","rotfl","rotflmfao","rotflol","ruin","ruined","ruining","ruins","sabotage","sad","sadden","saddened","sadly","safe","safely","safety","salient","sappy","sarcastic","satisfied","save","saved","scam","scams","scandal","scandalous","scandals","scapegoat","scapegoats","scare","scared","scary","sceptical","scold","scoop","scorn","scornful","scream","screamed","screaming","screams","screwed","screwed up","scumbag","secure","secured","secures","sedition","seditious","seduced","self-confident","self-deluded","selfish","selfishness","sentence","sentenced","sentences","sentencing","serene","severe","sexy","shaky","shame","shamed","shameful","share","shared","shares","shattered","shit","shithead","shitty","shock","shocked","shocking","shocks","shoot","short-sighted","short-sightedness","shortage","shortages","shrew","shy","sick","sigh","significance","significant","silencing","silly","sincere","sincerely","sincerest","sincerity","sinful","singleminded","skeptic","skeptical","skepticism","skeptics","slam","slash","slashed","slashes","slashing","slavery","sleeplessness","slick","slicker","slickest","sluggish","slut","smart","smarter","smartest","smear","smile","smiled","smiles","smiling","smog","sneaky","snub","snubbed","snubbing","snubs","sobering","solemn","solid","solidarity","solution","solutions","solve","solved","solves","solving","somber","some kind","son-of-a-bitch","soothe","soothed","soothing","sophisticated","sore","sorrow","sorrowful","sorry","spam","spammer","spammers","spamming","spark","sparkle","sparkles","sparkling","speculative","spirit","spirited","spiritless","spiteful","splendid","sprightly","squelched","stab","stabbed","stable","stabs","stall","stalled","stalling","stamina","stampede","startled","starve","starved","starves","starving","steadfast","steal","steals","stereotype","stereotyped","stifled","stimulate","stimulated","stimulates","stimulating","stingy","stolen","stop","stopped","stopping","stops","stout","straight","strange","strangely","strangled","strength","strengthen","strengthened","strengthening","strengthens","stressed","stressor","stressors","stricken","strike","strikers","strikes","strong","stronger","strongest","struck","struggle","struggled","struggles","struggling","stubborn","stuck","stunned","stunning","stupid","stupidly","suave","substantial","substantially","subversive","success","successful","suck","sucks","suffer","suffering","suffers","suicidal","suicide","suing","sulking","sulky","sullen","sunshine","super","superb","superior","support","supported","supporter","supporters","supporting","supportive","supports","survived","surviving","survivor","suspect","suspected","suspecting","suspects","suspend","suspended","suspicious","swear","swearing","swears","sweet","swift","swiftly","swindle","swindles","swindling","sympathetic","sympathy","tard","tears","tender","tense","tension","terrible","terribly","terrific","terrified","terror","terrorize","terrorized","terrorizes","thank","thankful","thanks","thorny","thoughtful","thoughtless","threat","threaten","threatened","threatening","threatens","threats","thrilled","thwart","thwarted","thwarting","thwarts","timid","timorous","tired","tits","tolerant","toothless","top","tops","torn","torture","tortured","tortures","torturing","totalitarian","totalitarianism","tout","touted","touting","touts","tragedy","tragic","tranquil","trap","trapped","trauma","traumatic","travesty","treason","treasonous","treasure","treasures","trembling","tremulous","tricked","trickery","triumph","triumphant","trouble","troubled","troubles","true","trust","trusted","tumor","twat","ugly","unacceptable","unappreciated","unapproved","unaware","unbelievable","unbelieving","unbiased","uncertain","unclear","uncomfortable","unconcerned","unconfirmed","unconvinced","uncredited","undecided","underestimate","underestimated","underestimates","underestimating","undermine","undermined","undermines","undermining","undeserving","undesirable","uneasy","unemployment","unequal","unequaled","unethical","unfair","unfocused","unfulfilled","unhappy","unhealthy","unified","unimpressed","unintelligent","united","unjust","unlovable","unloved","unmatched","unmotivated","unprofessional","unresearched","unsatisfied","unsecured","unsettled","unsophisticated","unstable","unstoppable","unsupported","unsure","untarnished","unwanted","unworthy","upset","upsets","upsetting","uptight","urgent","useful","usefulness","useless","uselessness","vague","validate","validated","validates","validating","verdict","verdicts","vested","vexation","vexing","vibrant","vicious","victim","victimize","victimized","victimizes","victimizing","victims","vigilant","vile","vindicate","vindicated","vindicates","vindicating","violate","violated","violates","violating","violence","violent","virtuous","virulent","vision","visionary","visioning","visions","vitality","vitamin","vitriolic","vivacious","vociferous","vulnerability","vulnerable","walkout","walkouts","wanker","want","war","warfare","warm","warmth","warn","warned","warning","warnings","warns","waste","wasted","wasting","wavering","weak","weakness","wealth","wealthy","weary","weep","weeping","weird","welcome","welcomed","welcomes","whimsical","whitewash","whore","wicked","widowed","willingness","win","winner","winning","wins","winwin","wish","wishes","wishing","withdrawal","woebegone","woeful","won","wonderful","woo","woohoo","wooo","woow","worn","worried","worry","worrying","worse","worsen","worsened","worsening","worsens","worshiped","worst","worth","worthless","worthy","wow","wowow","wowww","wrathful","wreck","wrong","wronged","wtf","yeah","yearning","yeees","yes","youthful","yucky","yummy","zealot","zealots","zealous",
    NULL
};

static int sentiment_valence[] = {
    -2,-2,-2,-2,-2,-2,-3,-3,-3,-3,2,2,1,-1,-1,2,2,2,2,1,-3,-3,-3,-3,1,1,1,1,-2,-2,-2,-2,2,2,2,-2,-2,-2,-2,-2,-2,-2,1,-2,2,2,2,2,-3,1,1,3,3,3,3,-1,-1,-1,-2,-2,1,1,3,3,3,3,1,2,2,2,2,2,-1,3,3,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,2,-3,-3,-3,-3,-3,-3,-3,-3,1,2,1,1,1,-2,-2,-2,-2,-1,-1,-2,1,-2,1,-2,2,2,2,4,2,-1,3,3,3,3,-3,-3,-3,-3,-3,-2,-2,-2,-2,-2,-2,-2,-1,1,-2,-2,-3,-3,-3,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,-2,2,2,2,1,-2,-3,-2,-2,-2,-2,-4,-3,-3,2,2,-4,-4,2,3,3,3,3,3,-1,-1,-1,-1,1,1,2,2,2,1,3,1,-1,-1,-1,2,-1,-1,-1,-1,-1,-1,3,3,3,4,-3,-2,-1,-1,1,2,1,-3,-3,-3,-2,-2,-2,-2,-2,-1,-3,-3,-2,2,-2,-5,-5,-1,-1,-2,3,-1,3,3,3,3,-2,-2,3,2,2,2,2,-2,-2,-2,-2,3,-3,-3,-3,-3,-3,2,-1,-2,1,-5,-5,-2,-2,-2,-2,-2,-2,-2,-2,2,2,3,-1,3,3,2,-1,3,-1,-1,-1,-3,-2,-2,2,2,-1,1,1,1,1,-2,-2,-3,-2,-2,-2,-2,-2,-2,-2,-2,-3,2,3,5,-3,1,2,1,4,2,-1,-1,-2,-2,-4,-2,-2,-2,2,-2,-2,-2,-2,2,2,2,2,-3,-1,-1,-1,-1,-1,1,3,2,1,2,2,-2,2,-2,-2,-3,-4,-1,3,3,3,3,-2,-2,-2,1,-2,-2,-1,2,2,-2,-2,-3,-2,3,3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,2,2,2,2,-2,2,3,2,2,2,2,2,-2,-1,-2,-2,-2,-2,2,2,-2,3,2,2,1,1,1,1,2,-1,-2,-5,-5,-5,-2,-2,-2,-2,-2,-2,-1,-1,-1,-2,-2,-3,-1,-1,1,2,2,2,2,2,2,1,2,1,1,1,2,1,2,2,-2,-2,-2,-2,2,2,2,2,2,-2,-2,-2,-2,2,2,-2,-2,-2,-2,-2,-2,-2,2,2,2,2,2,2,2,-3,-2,-2,-2,-1,-2,-2,-2,-1,-1,-1,-2,-2,-2,-2,1,1,1,2,1,3,-2,-1,-2,2,2,2,2,-3,-2,-2,2,-1,-3,-2,-2,-2,-2,2,-2,-2,-2,-3,-3,-3,-3,-2,-2,-2,-2,-2,-2,-2,-3,-3,-1,-2,-1,-1,-1,-2,-5,1,-1,-1,2,-1,-1,-2,-2,-2,-3,-3,-4,-4,-4,-2,2,2,-2,-1,2,-3,-2,-1,2,3,-2,2,-2,-3,-3,-3,-3,-3,-3,-3,1,2,-2,-3,-3,2,2,-2,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,3,3,3,3,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,2,1,2,2,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-1,-2,-2,-2,2,-2,-2,-2,3,1,-4,-4,-3,-3,-1,-2,-1,-3,-3,-3,-2,-2,-2,-2,-1,-2,-2,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-3,-2,-1,-1,-1,-1,-2,-2,-2,-2,-1,-2,-2,-2,-2,-2,-1,-1,-1,-1,-3,-3,-3,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-3,-3,-2,-2,-2,-2,-2,-1,-2,-2,-3,-2,-2,-2,-2,-1,-1,-1,-1,-1,-3,-3,-2,-2,-2,-1,-1,-1,-2,-2,-2,-3,-2,1,1,-2,-2,-1,-2,-2,-2,-2,-2,-2,-2,-3,-3,-1,-2,-1,-2,-2,-2,2,2,2,1,4,-2,-2,2,2,3,3,2,2,-2,-2,-2,-2,-2,-2,1,-2,2,-1,-1,2,2,2,2,2,2,2,2,2,-2,-2,2,1,1,1,2,2,2,2,2,2,2,-2,-2,-2,-2,-2,3,-2,-2,-2,1,1,1,2,3,3,1,2,-1,-2,-1,-1,-2,-2,-2,-1,-1,-1,2,2,3,4,-1,-3,-2,-2,-2,-2,2,3,3,3,3,3,3,-1,-2,-1,2,-1,-1,-2,3,3,3,2,2,2,2,1,1,-2,-2,-2,-2,-2,-2,-2,-2,1,1,-1,-1,-1,-1,1,1,4,3,3,4,-2,-3,-3,-3,-2,-2,-2,-2,-2,-2,-2,2,1,3,-3,-3,-3,-2,-1,-3,-3,1,3,4,-1,3,3,3,3,-2,-2,-3,-3,-2,-2,-2,-2,2,2,2,2,2,2,-2,-2,-2,2,-2,-3,-2,1,-3,-3,2,2,2,-3,-2,-1,2,-2,-2,-2,1,1,2,-1,-2,-2,-2,-2,2,2,2,-2,-2,-2,-1,-2,-2,-1,-2,1,1,-1,2,-1,-4,-4,-4,-4,-4,-4,1,2,-3,1,2,-2,-2,-3,-2,2,-1,-2,-2,-2,-2,-2,3,-4,-4,-4,-4,-4,-4,-4,-4,-3,-4,-4,2,2,2,-2,4,-1,-1,2,4,4,-3,2,-2,-2,2,2,2,2,3,3,3,2,3,-1,-2,2,3,3,3,3,3,-1,-2,2,2,-2,1,-3,4,3,3,1,3,3,1,1,1,1,3,2,-2,-1,3,3,3,-3,-2,-3,-3,-3,-3,-3,-3,1,1,1,2,1,-1,-2,-2,-2,1,2,1,-3,-3,-2,-2,-1,2,-1,3,3,3,2,2,-2,-2,3,3,-1,2,-2,2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-3,-3,-3,-3,-3,-1,-2,1,-1,-2,2,-3,-3,3,2,4,-2,-4,2,2,2,-2,2,2,2,3,-2,-2,-1,-1,-1,-1,2,2,-2,-2,-2,2,2,2,2,2,2,2,-2,-2,-2,2,2,2,-2,-2,2,2,-3,-3,-3,-3,-2,-2,2,1,2,3,-3,-3,2,2,2,2,-2,5,-2,-2,-2,-2,-3,-3,-3,-3,-3,-2,-2,-1,-2,-1,-2,-3,-2,-2,-2,-3,-1,2,1,-2,-2,2,2,-1,-1,-1,-1,-2,3,3,3,3,-2,2,2,2,2,2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,1,1,-2,2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,2,2,-2,-2,-2,2,-2,-2,-2,-2,-2,-1,-2,-2,-2,1,1,1,2,-2,2,-2,-2,-2,-2,-2,-2,-2,2,2,2,2,2,3,-2,-2,-2,-2,2,2,2,1,1,2,2,1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,2,1,2,1,1,2,-3,-1,-1,-1,2,-2,2,-1,-3,-3,-3,-1,-2,-4,-4,-2,2,-2,-2,-3,1,1,1,2,1,2,2,2,2,3,3,3,-2,3,3,-1,2,2,2,1,-3,-3,-3,-3,2,2,2,3,-2,-2,-1,-2,-2,-2,-2,2,1,1,1,1,1,1,3,-2,-2,-1,-1,-1,-1,1,1,1,-2,-2,-3,-3,-2,-2,4,1,2,2,2,-1,-1,-1,-1,-2,2,-2,4,4,-3,-3,-3,-3,-2,-2,3,-2,-2,-1,-1,-1,-1,-1,-3,-3,-3,-3,-3,-3,3,3,3,3,3,2,-1,3,3,3,3,3,-2,-3,-3,-1,-1,-1,-3,-3,-1,-3,-3,-1,-1,-1,-1,3,3,3,4,4,1,1,2,2,-2,3,-3,1,-2,-2,-2,2,3,-2,-2,-2,2,-2,4,3,3,3,-2,-2,-2,-2,-1,-1,-3,-2,-2,-2,-2,-2,-3,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-3,-5,-5,1,2,2,1,-2,-2,-2,-2,-2,-2,-2,-2,-3,-3,-2,-1,-2,-2,-3,1,-2,-2,-2,-2,-2,-2,-2,-2,-1,-2,-2,3,2,-5,-5,-1,-3,2,-1,-2,-2,-2,-2,-3,-2,2,-1,-3,-2,-2,-3,-2,2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,2,3,3,2,2,-2,-2,2,2,-2,-2,-2,-3,-3,2,5,4,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-2,-2,-3,-3,-3,3,-1,2,2,2,2,-1,2,-1,-1,-2,-1,2,2,2,-2,-1,3,2,3,2,-2,-3,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,2,-1,-2,-2,-4,-4,-3,-2,-1,-2,2,3,1,3,3,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,3,2,2,-2,-1,-1,-1,-1,-1,2,-2,3,3,3,3,1,1,1,-2,-2,1,-1,-2,-1,-1,-1,1,-1,-1,-1,-1,-5,-2,-2,-2,2,2,-2,-2,-2,2,2,1,1,1,1,1,1,1,-2,-1,-2,-1,-1,1,1,3,1,1,1,-2,-2,-2,-2,2,2,-1,-1,-1,-1,-3,-2,-2,-2,-2,-1,-2,-2,-2,-1,-1,-3,-3,-3,-2,-2,-1,-3,-3,-3,-3,-4,-4,2,2,2,4,-2,2,1,1,1,1,1,1,1,2,-2,-2,-2,2,2,2,2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,4,4,4,4,2,-1,2,1,2,1,2,2,2,-2,-1,-2,2,2,2,-2,-1,-1,-1,-1,2,2,2,2,2,2,2,2,2,-2,1,1,1,1,-2,-2,-2,-2,-2,-1,-2,-2,-1,-2,-2,2,2,2,2,2,2,2,2,-3,-1,-1,3,3,3,-2,-2,-2,-2,-2,-2,-2,-2,-2,2,4,4,4,2,4,4,4,-2,-2,-2,-2,-2,-2,-2,-2,-2,1,1,1,1,-1,-2,2,2,2,-2,-2,-3,-3,-3,-2,-2,-2,-2,-2,-2,-2,3,-2,-2,-2,-2,-2,-2,-2,-3,-4,2,2,2,-2,-2,-1,2,-2,-3,-3,-2,-2,-2,-2,2,-2,3,-2,-2,-2,-2,1,1,1,-2,-4,-4,-3,-2,-2,-2,-2,-1,-2,-2,-2,-2,-4,-1,-2,-2,1,1,-1,-1,2,2,2,2,-3,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-3,-2,2,2,2,-2,-5,1,2,2,-2,2,2,2,2,-2,-1,-2,-2,-2,-2,1,-1,2,2,1,1,1,1,1,1,-2,0,-5,3,3,3,2,-1,-2,-2,-1,-2,-3,-3,-2,1,3,3,3,-2,1,2,-2,-2,3,2,-1,-2,-2,2,-2,-2,-2,-2,2,-2,-2,-2,-2,-2,-2,2,-2,-2,-2,-2,-1,1,1,1,2,-2,-2,-1,-1,-1,-1,2,1,-1,-1,-2,2,2,2,2,2,-2,-2,-2,-2,-1,-2,-1,2,2,2,-1,-2,-2,-2,-2,-2,-2,-2,4,-2,-2,2,1,1,-2,2,3,-3,-3,-2,-2,-2,-2,-2,-2,-2,-2,-2,2,3,5,2,2,2,1,1,1,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,2,2,2,-3,-3,-3,2,2,-2,-2,2,-2,-1,-3,-3,4,-3,-3,-3,-3,-3,2,2,2,-2,2,-2,-2,-2,-2,-2,-2,-2,5,-2,-2,-2,-2,-2,-2,-2,-2,2,-2,2,2,-2,-4,-4,-4,-4,-2,-2,-2,-2,-2,-2,-2,-2,2,-1,-2,-3,-3,-2,-3,-3,2,2,-2,-2,-2,-2,4,4,-2,-2,-2,2,1,2,-2,-5,-3,-2,-2,-2,-2,-1,-1,2,-1,-1,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-1,2,-2,-2,-2,-2,-2,-2,1,-2,-2,1,-2,-2,-2,1,-2,-2,-2,-2,-2,-1,-2,-2,2,-2,-1,2,-2,-2,-2,-2,-2,-2,-1,2,2,-2,-2,-2,1,1,1,1,-1,-1,1,-2,-2,3,-2,-3,-3,-3,-3,-3,-3,3,-3,2,2,2,2,-2,-2,-2,-2,-3,-3,2,-2,1,3,1,1,3,1,-3,3,-1,-2,-2,-2,-2,-3,1,-2,-2,1,2,-2,-2,-3,-3,-2,-1,-2,-2,-1,-2,-2,3,2,-2,-2,-2,-2,2,2,2,1,-3,-4,-2,-1,2,4,4,4,4,3,1,1,1,-3,-2,-3,3,4,3,3,4,4,-1,-3,-3,-3,-3,-3,-3,-3,-3,3,-3,2,-2,2,4,4,4,-3,-2,-2,-2,-4,1,1,2,1,2,-2,3,-2,-2,2
};

/* set the language here */
char ** egocentrism_lang = egocentrism_word_en;
char ** sentiment_lang = sentiment_word_en;
char ** gender_lang = gender_word_en;
char ** trigger_lang = trigger_word_en;

/**
 * @brief Sets the current language
 * @param lang Two characters indicating the language
 */
void set_language(char * lang)
{
    egocentrism_lang = egocentrism_word_en;
    sentiment_lang = sentiment_word_en;
    gender_lang = gender_word_en;
    trigger_lang = trigger_word_en;

    if (strcmp(lang, "de") == 0) {
        /* TODO */
        return;
    }

    if (strcmp(lang, "es") == 0) {
        /* TODO */
        return;
    }

    if (strcmp(lang, "fr") == 0) {
        /* TODO */
        return;
    }
}

/**
 * @brief Detects whether the given character is a terminator
 * @param c The character
 * @return Zero if this is not a terminator, 1 otherwise.
 */
int terminating_char(char c)
{
    if ((c == ' ') || (c == ',') ||
        (c == '.') || (c == ';'))
        return 1;
    return 0;
}

/**
 * @brief Returns a value indicating the valence of the given text
 * @param str The text to be analysed
 * @return Value indicating valence
 */
int get_valence(char * str)
{
    int i, j, ctr=0, valence=0;
    char word[256];

    for (i = 0; i < strlen(str); i++) {
        if (terminating_char(str[i]) == 1) {
            if (ctr > 1) {
                word[ctr] = 0;
                j = 0;
                while (sentiment_lang[j] != NULL) {
                    if (strlen(word) == strlen(sentiment_lang[j])) {
                        if (strcmp(word, sentiment_lang[j]) == 0) {
                            valence += sentiment_valence[j];
                            break;
                        }
                    }
                    j++;
                }
            }
            ctr = 0;
        }
        else {
            if (ctr < 254)
                word[ctr++] = tolower(str[i]);
        }
    }

    if (ctr > 1) {
        word[ctr] = 0;
        j = 0;
        while (sentiment_lang[j] != NULL) {
            if (strlen(word) == strlen(sentiment_lang[j])) {
                if (strcmp(word, sentiment_lang[j]) == 0) {
                    valence += sentiment_valence[j];
                    break;
                }
            }
            j++;
        }
    }

    return valence;
}

/**
 * @brief Returns a value indicating egocentrism/extroversion
 * @param str The text to be analysed
 * @return Value indicating egotism/extroversion, with egotism in the positive direction
 */
int get_egocentrism(char * str)
{
    int i, j, ctr=0, egocentrism=0;
    char word[256];

    for (i = 0; i < strlen(str); i++) {
        if (terminating_char(str[i]) == 1) {
            if (ctr > 0) {
                word[ctr] = 0;
                j = 0;
                while (egocentrism_lang[j] != NULL) {
                    if (strlen(word) == strlen(egocentrism_lang[j])) {
                        if (strcmp(word, egocentrism_lang[j]) == 0) {
                            egocentrism += egocentrism_value[j];
                            break;
                        }
                    }
                    j++;
                }
            }
            ctr = 0;
        }
        else {
            if (ctr < 254)
                word[ctr++] = tolower(str[i]);
        }
    }

    if (ctr > 0) {
        word[ctr] = 0;
        j = 0;
        while (egocentrism_lang[j] != NULL) {
            if (strlen(word) == strlen(egocentrism_lang[j])) {
                if (strcmp(word, egocentrism_lang[j]) == 0) {
                    egocentrism += egocentrism_value[j];
                    break;
                }
            }
            j++;
        }
    }

    return egocentrism;
}

/**
 * @brief Returns a value indicating the masculine/feminine content of the given text
 * @param str The text to be analysed
 * @return Value indicating masuline/feminine, with feminine in the positive direction
 */
int get_gender(char * str)
{
    int i, j, ctr=0, gender=0, name_gender;
    char word[256];

    for (i = 0; i < strlen(str); i++) {
        if (terminating_char(str[i]) == 1) {
            if (ctr > 0) {
                word[ctr] = 0;
                if (strlen(word) > 1) {
                    j = 0;
                    while (gender_lang[j] != NULL) {
                        if (strlen(word) == strlen(gender_lang[j])) {
                            if (strcmp(word, gender_lang[j]) == 0) {
                                gender += gender_value[j];
                                break;
                            }
                            else {
                                if (strlen(word) > 2) {
                                    name_gender = get_gender_from_name(word);
                                    if (name_gender == GENDER_FEMININE) gender+=2;
                                    if (name_gender == GENDER_MASCULINE) gender-=2;
                                }
                            }
                        }
                        j++;
                    }
                }
            }
            ctr = 0;
        }
        else {
            if (ctr < 254)
                word[ctr++] = tolower(str[i]);
        }
    }

    if (ctr > 0) {
        word[ctr] = 0;
        if (strlen(word) > 1) {
            j = 0;
            while (gender_lang[j] != NULL) {
                if (strlen(word) == strlen(gender_lang[j])) {
                    if (strcmp(word, gender_lang[j]) == 0) {
                        gender += gender_value[j];
                        break;
                    }
                    else {
                        if (strlen(word) > 2) {
                            name_gender = get_gender_from_name(word);
                            if (name_gender == GENDER_FEMININE) gender+=2;
                            if (name_gender == GENDER_MASCULINE) gender-=2;
                        }
                    }
                }
                j++;
            }
        }
    }

    return gender;
}

/**
 * @brief Reads triggers from a custom file /var/lib/gnu-slash-mood/triggers.txt
 * @param str The text to be analysed
 * @return Zero or greater trigger word detections, -1 if the file doesn't exist
 */
int read_trigger_words(char * str)
{
    FILE * fp;
    char linestr[256], custom_trigger_word[256], word[256];
    int i, j, ctr=0, trigger=0;

    fp = fopen("/var/lib/gnu-slash-mood/triggers.txt", "r");
    if (!fp) return -1;

    while (!feof(fp)) {
        if (fgets(linestr , 254 , fp) != NULL ) {
            if (strlen(linestr) == 0) continue;

            /* extract the trigger word */
            j = 0;
            for (i = 0; i < strlen(linestr); i++)
                if ((linestr[i] != 10) && (linestr[i] != 13) &&
                    (linestr[i] != ' ') && (linestr[i] != ','))
                    custom_trigger_word[j++] = tolower(linestr[i]);

            /* string terminator */
            custom_trigger_word[j] = 0;

            /* is the trigger word within the text? */
            ctr = 0;
            for (i = 0; i < strlen(str); i++) {
                if ((terminating_char(str[i]) == 1) || (i == strlen(str)-1)) {
                    if (i == strlen(str)-1)
                        word[ctr++] = tolower(str[i]);
                    if (ctr > 0) {
                        /* string terminator */
                        word[ctr] = 0;
                        if (strlen(word) > 1) {
                            if (strlen(word) == strlen(custom_trigger_word)) {
                                if (strcmp(word, custom_trigger_word) == 0) {
                                    trigger++;
                                    break;
                                }
                            }
                        }
                    }
                    ctr = 0;
                }
                else {
                    if (ctr < 254)
                        word[ctr++] = tolower(str[i]);
                }
            }

        }
    }

    fclose(fp);
    return trigger;
}

/**
 * @brief Returns the number of trigger words found
 * @param str The text to be analysed
 * @return Number of matching trigger words
 */
int get_triggers(char * str)
{
    int i, j, ctr=0, trigger=0, retval;
    char word[256];

    /* if custom trigger words exist then read them */
    retval = read_trigger_words(str);
    if (retval != -1) return retval;

    for (i = 0; i < strlen(str); i++) {
        if (terminating_char(str[i]) == 1) {
            if (ctr > 0) {
                word[ctr] = 0;
                if (strlen(word) > 1) {
                    j = 0;
                    while (trigger_lang[j] != NULL) {
                        if (strlen(word) == strlen(trigger_lang[j])) {
                            if (strcmp(word, trigger_lang[j]) == 0) {
                                trigger++;
                                break;
                            }
                        }
                        j++;
                    }
                }
            }
            ctr = 0;
        }
        else {
            if (ctr < 254)
                word[ctr++] = tolower(str[i]);
        }
    }

    if (ctr > 0) {
        word[ctr] = 0;
        if (strlen(word) > 1) {
            j = 0;
            while (trigger_lang[j] != NULL) {
                if (strlen(word) == strlen(trigger_lang[j])) {
                    if (strcmp(word, trigger_lang[j]) == 0) {
                        trigger++;
                        break;
                    }
                }
                j++;
            }
        }
    }

    return trigger;
}

/**
 * @brief Convert sentiment values to rgb values in the range 0-1000
 * @param str The text to be subjected to sentiment analysis
 * @param r Returned red colour in the range 0-1000
 * @param g Returned green colour in the range 0-1000
 * @param b Returned blue colour in the range 0-1000
 * @param frequency Returned pulsation frequency in seconds
 */
void sentiment_to_rgb(char * str, int * r, int * g, int * b,
                      int * frequency)
{
    /* possibly these might need to be weighted */
    int valence = get_valence(str);
    int egocentrism = get_egocentrism(str);
    int gender = get_gender(str);
    int triggers = get_triggers(str);

    int max = abs(valence);
    const int maxval = 500;

    /* get the maximum value */
    if (abs(egocentrism) > max) max = abs(egocentrism);
    if (abs(gender) > max) max = abs(gender);

    /* normalise within the range 0-1000 */
    *b = maxval - (valence * maxval / max);
    *g = maxval + (egocentrism * maxval / max);
    *r = maxval + (gender * maxval / max);

    *frequency = 0;
    if (triggers > 0) {
        *frequency = 40 + (40-(triggers*16));
        if (*frequency < 20) *frequency = 20;
    }
}
