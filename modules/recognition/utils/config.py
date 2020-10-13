# -*- coding: utf-8 -*-

import os
import time
import json
from dotmap import DotMap
from easydict import EasyDict

def get_config_from_json(json_file):
	"""
	Get the config from a json file
	:param json_file
	:return: config(namespace) or config(dictionary)
	"""

	# parse the configurations from the config json file provided
	with open(json_file, 'r') as config_file:
		config_dict = json.load(config_file)

	# convert the dictionary to a namespace using bunch lib
	config = DotMap(config_dict)

	config.json_file = json_file
	config.json_string = str(config_dict)
	config.dict = getKeyValuePair(config_dict)

	return config, config_dict

def process_config(json_file):
	config, _ = get_config_from_json(json_file)
	config.callbacks.tensorboard_log_dir = os.path.join("experiments", time.strftime("%Y-%m-%d", time.localtime()), config.exp.name, "logs/")
	config.callbacks.checkpoint_dir = os.path.join("experiments", time.strftime("%Y-%m-%d", time.localtime()), config.exp.name, "checkpoints/")
	return config

def getKeyValuePair(dic, master_dic={}, master_key=None):
	keys = list(dic.keys())

	for key in keys:
		if type(dic[key]) == dict:
			getKeyValuePair(dic[key], master_dic=master_dic, master_key=key)
		else:
			if master_key == None:
				master_dic[key] = dic[key]
			else:
				master_dic['Config.'+str(master_key)+'.'+str(key)] = dic[key]
	return master_dic

def optTransplator(config):
	try:
		cudasetting = config.model.gpu
	except:
		cudasetting = True

	try:
		upper_case_only = config.data.upper_case_only
	except:
		upper_case_only = False

	opt = EasyDict({'experiment_name':config.exp.name,
					'train_data':'',
					'valid_data':config.META_DB.dbvalid,
					'manualSeed':1111,
					'workers':config.data.num_reading_workers,
					'batch_size':config.trainer.batch_size,
					'num_iter':config.trainer.num_epochs,
					'valInterval':config.trainer.valid_interval,
					'continue_model':config.model.pretrained_model_path,
					'saved_model':config.model.pretrained_model_path,
					'adam':False,
					'adadelta':False,
					'sgd':False,
					'lr':config.model.learning_rate,
					'beta1':config.model.beta1,
					'rho':config.model.rho,
					'eps':config.model.eps,
					'grad_clip':config.model.grad_clip,
					'select_data':config.META_DB.dblist,
					'target_prefix':config.META_DB.target_prefix,
					'batch_ratio':config.META_DB.batch_ratio,
					'total_data_usage_ratio':1,
					'batch_max_length':config.data.batch_max_length,
					'imgH':config.data.image_size_h,
					'imgW':config.data.image_size_w,
					'rgb':False,
					'character':config.data.character,
					'PAD':config.data.PAD,
					'data_filtering_off':True,
					'Transformation':config.model.transformation,
					'FeatureExtraction':config.model.featureExtraction,
					'SequenceModeling':config.model.sequenceModeling,
					'Prediction':config.model.prediction,
					'num_fiducial':config.model.num_fiducial,
					'input_channel':config.model.fe_input_channel,
					'output_channel':config.model.fe_output_channel,
					'hidden_size':config.model.bl_hidden_size,
					'upper_case_only':upper_case_only,
					'cuda':cudasetting,
					'num_gpu':1})

	if config.model.optimizer == 'Adam':
		opt.adam = True
	elif config.model.optimizer == 'Adadelta':
		opt.adadelta = True
	elif config.model.optimizer == 'SGD':
		opt.sgd = True

	if config.data.character == 'usecase1':
		opt.character = """wIyzu"5U r$Yg0,|e'isPVSvn9aZ4WmA7k6o8KXQb/~@#tLqC_DN;=*HlF%p?cB!3TjRGf.hM+d`&OxJ2^1E-:\(\)\[\]\{\}\<\>"""
	elif config.data.character == 'uppercase':
		opt.character = """!"#$%&'*+,./0123456789;=?@ABCDEFGHIJKLMNOPQRSTUVWXYZ^_`|~\-:\(\)\[\]\{\}\<\> """
	elif config.data.character == 'default':
		opt.character = """wIyzu"5U r$Yg0,|e'isPVSvn9aZ4WmA7k6o8KXQb/~@#tLqC_DN;=*HlF%p?cB!3TjRGf.hM+d`&OxJ2^1E\-\:\(\)\[\]\{\}\<\>"""
	elif config.data.character == 'ko':
		opt.character = """wIyzu"5U r$Yg0,|e'isPVSvn9aZ4WmA7k6o8KXQb/~@#tLqC_DN;=*HlF%p?cB!3TjRGf.hM+d`&OxJ2^1E\-\:\(\)\[\]\{\}\<\>가각갂갃간갇갈갉갊갋갌감갑값갓갔강갖갗같갚갛개객갠갢갣갤갬갭갯갰갱갲갵갶갸갹갼갿걀걁걇걈걉걋걌걍걔걘걜걤걥거걱걳건걵걷걸걹걺검겁것겄겅겆겇겉겊겋게겍겐겓겔겜겝겟겠겡겥겧겨격겪견겯결겱겴겷겸겹겻겼경겾겿곁곂계곅곈곋곌곔곕곗곘곙고곡곤곧골곩곪곫곬곮곯곰곱곳공곶곷곹곺곻과곽관괃괄괆괌괍괏괐광괘괙괜괟괠괢괨괩괫괬괭괴괵괸괻괼괾굄굅굇굈굉교굑굔굗굘굠굡굣굥구국군굳굴굵굶굸굻굼굽굿궁궂궃궆궈궉권궏궐궘궙궛궜궝궤궥궨궫궬궴궵궷궸궹귀귁귄귇귈귊귐귑귓귔귕규귝균귤귨귬귭귯귱그극근귾귿글긁긂긃긄긇금급긋긍긎긏긑긒긓긔긕긘긜긠긤긥긧긩기긱긴긷길긹긺긻긼김깁깃깄깅깆깇깉깊깋까깍깎깐깓깔깖깜깝깟깠깡깢깣깥깨깩깪깬깯깰깳깶깸깹깻깼깽꺁꺄꺅꺈꺋꺌꺍꺗꺙꺠꺵꺼꺽꺾껀껂껃껄껌껍껏껐껑껒껓껕께껙껚껜껟껠껨껩껫껬껭껱껴껵껸껻껼꼄꼅꼇꼈꼉꼍꼐꼗꼥꼬꼭꼰꼱꼲꼳꼴꼶꼸꼻꼼꼽꼿꽁꽂꽃꽅꽇꽈꽉꽌꽏꽐꽘꽙꽛꽜꽝꽤꽥꽨꽫꽬꽴꽵꽸꽹꾀꾁꾄꾇꾈꾐꾑꾓꾔꾕꾜꾠꾣꾤꾬꾭꾲꾸꾹꾼꾿꿀꿁꿇꿈꿉꿋꿍꿎꿏꿔꿕꿘꿛꿜꿤꿥꿧꿨꿩꿰꿱꿴꿷꿸뀀뀁뀃뀄뀅뀌뀍뀐뀓뀔뀜뀝뀟뀡뀨뀬뀰뀸뀹끄끅끈끊끋끌끍끎끓끔끕끗끙끛끝끟끠끡끧끨끫끳끵끼끽낀낃낄낋낌낍낏낐낑낕나낙낚낛난낟날낡낢낤남납낪낫났낭낮낯낰낱낲낳내낵낸낻낼냄냅냇냈냉냎냐냑냔냗냘냠냡냣냥냦냩냬넁너넉넊넋넌넏널넒넓넗넘넙넛넜넝넞넠넡넢넣네넥넨넫넬넴넵넷넸넹넼넽넾넿녀녁녃년녇녈녋념녑녓녔녕녘녙녚녛녜녝녠녣녤녬녭녯녱노녹녺녻논녾녿놀놂놈놉놋농놎놐놑높놓놔놕놘놛놜놤놥놧놨놩놰놴놸뇄뇌뇍뇐뇓뇔뇜뇝뇟뇠뇡뇦뇨뇩뇬뇯뇰뇸뇹뇻뇽누눅눈눋눌눍눐눓눔눕눗눙눛눞눠눡눤눧눨눰눱눳눴눵눼눽뉀뉃뉄뉌뉍뉏뉐뉑뉘뉙뉜뉟뉠뉨뉩뉫뉭뉯뉴뉵뉷뉸뉻뉼늄늅늇늉늊늋느늑는늗늘늙늚늛늜늠늡늣능늦늧늪늫늬늭늰늳늴늼늽늿닀닁니닉닌닏닐닑닒닓닔님닙닛닝닞닠닢닣다닥닦단닫달닭닮닯닰닲닳담답닶닷닸당닺닻닼닽닾닿대댁댄댇댈댐댑댓댔댕댙댜댝댠댤댧댬댭댯댱댸댼덍더덕덖던덙덛덜덞덟덤덥덦덧덨덩덪덫덭덮덯데덱덴덷델뎀뎁뎃뎄뎅뎆뎊뎌뎍뎏뎐뎓뎔뎘뎜뎝뎟뎠뎡뎨뎩뎬뎰뎸뎹뎻뎽도독돆돇돈돋돌돎돏돐돓돔돕돗동돚돛돜돝돞돟돠돡돤돨돰돱돳돴돵돼됀됃됄됌됍됏됐되됙된됟될됤됨됩됫됬됭됴됵됸됻됼둄둅둇둉둏두둑둔둗둘둙둚둛둜둠둡둣둥둪둬둭둰둳둴둼둽둿뒀뒁뒈뒉뒌뒏뒐뒘뒙뒛뒜뒝뒤뒥뒨뒫뒬뒴뒵뒷뒸뒹듀듁듄듈듐듑듓듕드득든듣들듥듦듧듨듬듭듯등듸듹듼듿딀딈딉딋딍디딕딘딛딜딤딥딧딨딩딪딫딭딮딯따딱딲딴딷딸딹딺딻딿땀땁땃땄땅땋때땍땎땐땓땔땜땝땟땠땡땧땨땩땬땰땽떄떈떠떡떤떧떨떪떫떯떰떱떳떴떵떻떼떽뗀뗃뗄뗌뗍뗏뗐뗑뗘뗙뗜뗟뗨뗩뗫뗬뗭뗴뗸똅똉또똑똒똔똗똘똙똚똟똠똡똣똥똬똭똰똴뙁뙈뙉뙌뙏뙐뙘뙙뙛뙜뙤뙥뙨뙫뙬뙴뙵뙷뙸뙹뚀뚁뚜뚝뚠뚣뚤뚦뚧뚫뚬뚭뚯뚱뚵뚸뚼뚿뛋뛌뛔뛘뛛뛜뛤뛥뛨뛩뛰뛴뛷뛸뜀뜁뜃뜅뜌뜍뜔뜜뜡뜨뜩뜬뜯뜰뜲뜳뜷뜸뜹뜻뜽띄띅띈띌띔띕띙띠띡띤띧띨띰띱띳띵띻라락란랃랄랈람랍랏랐랑랒랓랔랕랖랗래랙랜랟랠램랩랫랬랭랰랱랲랴략랸랻랼럄럅럇량럐럣러럭런럲럳럴럼럽럾럿렀렁렂렄렆렇레렉렌렏렐렘렙렛렜렝렡렢렣려력련렫렬렰렴렵렷렸령렼렾례롁롄롇롈롐롑롓롕로록론롣롤롨롫롬롭롯롱롴롶롷롸롹롼뢀뢈뢉뢋뢌뢍뢔뢘뢛뢜뢧뢨뢰뢱뢴뢵뢷뢸룀룁룃룄룅료룍룐룓룔룜룝룟룡루룩룬룯룰룳룸룹룻룽뤀뤂뤄뤅뤈뤋뤌뤔뤕뤗뤘뤙뤠뤡뤤뤨뤰뤱뤳뤴뤵뤼뤽륀륃륄륌륍륏륐륑류륙륜륟률륨륩륫륭르륵른륻를륽릀름릅릇릉릊릋릍릎릏릐릑릔릘릞릠릡릣릥리릭린릳릴림립릾릿맀링맄맆마막맊만많맏말맑맒맔맘맙맛맜망맞맟맠맡맢맣매맥맧맨맫맬맭맴맵맷맸맹맺맻맽맾먀먁먄먈먐먑먓먕먘먜머먹먻먼먿멀멁멂멈멉멋멌멍멎멏멓메멕멘멛멜멠멤멥멧멨멩멪멫멭며멱면멷멸멺몀몁몃몄명몇몉몌몍몐몓몔몜몝몟몡몣몥몦모목몫몬몯몰몱몲몴몸몹못몽몿뫃뫄뫅뫈뫋뫌뫔뫕뫗뫘뫙뫠뫤뫨뫱뫴뫼뫽묀묃묄묌묍묏묑묘묙묜묟묠묨묩묫묭무묵묶문묺묻물묽묾뭀뭄뭅뭇뭉뭋뭍뭏뭐뭑뭔뭗뭘뭠뭡뭣뭤뭥뭬뭭뭰뭴뭼뭽뭿뮀뮁뮈뮉뮊뮌뮐뮘뮙뮛뮜뮝뮤뮥뮨뮫뮬뮴뮵뮷뮹므믁믃믄믇믈믉믌믏믐믑믓믕믙믜믝믠믤믬믭믯믱믲미믹민믿밀밁밂밄밈밉밋밌밍밎및밐밑바박밖밗반받발밝밞밟밠밣밤밥밧밨방밫밬밭밯배백밴밷밸뱀뱁뱃뱄뱅뱆뱉뱌뱍뱐뱔뱜뱝뱟뱡뱧뱨뱰뱷뱽버벅벆번벋벌벍벎벐범법벗벘벙벚벜벝베벡벢벤벧벨벰벱벳벴벵벸벹벼벽변볃별볈볌볍볏볐병볒볓볔볕볘볙볜볠볨볩볫볭보복볶볷본볻볼볽볿봄봅봇봉봊봋봍봏봐봑봔봗봘봠봡봣봤봥봬봰봳봴뵀뵈뵉뵊뵌뵏뵐뵘뵙뵛뵜뵝뵤뵥뵨뵬뵴뵵뵷뵹부북붂분붇불붉붊붋붎붐붑붓붕붖붗붘붙붚붛붜붝붠붣붤붩붪붫붬붭붯붰붱붴붸붹붼붿뷀뷈뷉뷋뷌뷍뷔뷕뷘뷛뷜뷤뷥뷧뷩뷰뷱뷴뷷뷸븀븁븃븅브븍븐븓블븕븗븘븛븜븝븟븡븣븥븧븨븩븬븰븸븹븻븽비빅빈빋빌빎빔빕빗빘빙빚빛빜빝빟빠빡빢빤빧빨빩빪빰빱빳빴빵빶빹빻빼빽빾뺀뺃뺄뺌뺍뺏뺐뺑뺘뺙뺜뺨뺴뺸뻐뻑뻔뻗뻘뻠뻡뻣뻤뻥뻦뻬뻭뻰뻳뻴뻼뻽뻿뼁뼈뼉뼌뼏뼐뼘뼙뼛뼜뼝뼟뼡뼤뼷뼸뼹뽀뽁뽄뽇뽈뽐뽑뽓뽕뽙뽛뽜뽠뽣뽸뽿뾔뾤뾥뾬뾰뿀뿁뿅뿌뿍뿐뿓뿔뿕뿜뿝뿟뿡뿥뿨뿯쀄쀠쀡쀼쁃쁄쁑쁘쁙쁜쁟쁠쁨쁩쁫쁴삐삑삔삗삘삠삡삣삤삥삦삧삫사삭삮삯산삳살삵삶삷삸삺삻삼삽삿샀상샂샃샄샅샆샇새색샋샌샏샐샘샙샛샜생샣샤샥샨샫샬샴샵샷샸샹샻샾샿섀섄섈섐섕서석섞섟선섣설섥섦섧섨섪섬섭섯섰성섴섶세섹센섿셀셈셉셋셌셍셎셑셒셔셕셗션셛셜셟셠셤셥셧셨셩셰셱셳셴셸솀솁솃솄솅소속솎손솓솔솕솖솗솘솜솝솟송솣솥솦솨솩솬솰솸솹솻솼솽쇄쇅쇈쇋쇌쇔쇕쇗쇘쇙쇠쇡쇤쇧쇨쇰쇱쇳쇴쇵쇼쇽숀숃숄숌숍숏숑숖수숙순숟술숡숤숧숨숩숫숭숮숯숱숲숳숴숵숸숻숼쉄쉅쉇쉈쉉쉐쉑쉔쉗쉘쉠쉡쉣쉤쉥쉬쉭쉰쉳쉴쉼쉽쉿슁슆슈슉슌슏슐슘슙슛슝스슥슨슫슬슭슮슯슰슲슳슴습슷승슺슻슾싀싁싄싈싐싑싓싕시식싞신싢싣실싥싦싨싫심십싯싰싱싳싴싶싸싹싻싼싿쌀쌁쌂쌈쌉쌋쌌쌍쌎쌑쌓쌔쌕쌘쌛쌜쌤쌥쌧쌨쌩쌯쌰쌱쌴쌷쌸썀썁썃썅썌썐썔썜썡써썩썪썬썯썰썲썸썹썻썼썽쎂쎄쎅쎈쎋쎌쎔쎕쎗쎘쎙쎝쎠쎡쎤쎧쎨쎰쎱쎳쎴쎵쎼쎽쏀쏌쏍쏏쏐쏑쏘쏙쏜쏟쏠쏢쏨쏩쏫쏭쏱쏴쏵쏸쏻쏼쐄쐅쐇쐈쐉쐐쐑쐔쐗쐘쐣쐤쐬쐭쐰쐳쐴쐼쐽쐿쑀쑁쑈쑉쑌쑏쑐쑘쑙쑛쑝쑤쑥쑨쑫쑬쑴쑵쑷쑸쑹쑾쒀쒁쒄쒇쒈쒐쒑쒓쒔쒕쒜쒝쒠쒣쒤쒬쒭쒯쒰쒱쒸쒹쒼쒿쓀쓈쓉쓋쓍쓓쓔쓕쓘쓛쓜쓤쓥쓧쓩쓰쓱쓴쓷쓸쓺쓿씀씁씃씅씌씍씐씔씜씝씟씡씨씩씬씯씰씱씸씹씻씼씽씿앀앃아악안앉않앋알앍앎앏앒앓암압앖앗았앙앚앛앝앞앟애액앤앧앨앰앱앳앴앵앹앺야약앾얀얃얄얇얌얍얏얐양얒얔얕얗얘얙얜얟얠얨얩얫얬얭어억얶언얹얺얻얼얽얾얿엄업없엇었엉엊엌엎엏에엑엔엗엘엠엡엣엤엥엩엪여역엮연엱엳열엶엷엸염엽엾엿였영옄옅옆옇예옉옌옏옐옘옙옛옜옝옠오옥옦온옪옫올옭옮옯옰옲옳옴옵옷옹옺옻옽옾와왁완왇왈왐왑왓왔왕왘왙왜왝왠왣왤왬왭왯왰왱외왹왼왿욀욂욈욉욋욌욍욒요욕욘욛욜욝욤욥욧용욬우욱운욷울욹욺욼움웁웂웃웅웆웇웉워웍원웒웓월웘웜웝웟웠웡웤웥웨웩웬웯웰웸웹웻웼웽윁위윅윈윋윌윎윓윔윕윗윘윙윜윝유육윤윧율윰윱윳융윶윷윸윹으윽은읃을읅읆읇읊음읍읎읏응읒읓읔읕읖읗의읙읜읟읠읦읨읩읫읭이익인읻일읽읾잀잂잃임입잆잇있잉잊잋잌잍잎잏자작잓잔잖잗잘잙잚잠잡잣잤장잦잧잩잪잫재잭잰잲잳잴잼잽잿쟀쟁쟂쟈쟉쟌쟎쟏쟐쟘쟙쟛쟝쟤쟨쟫쟬쟴쟵쟷쟹저적젂전젆젇절젉젊점접젓젔정젖젙젛제젝젠젣젤젬젭젯젰젱젲젶져젹젼젿졀졁졂졃졇졈졉졋졌졍졎졑졓졔졕졘졜졤졥졧졩조족존졷졸졺좀좁좃종좆좇좋좌좍좐좒좓좔좕좜좝좟좠좡좨좩좬좯좰좸좻좼좽죄죅죈죋죌죔죕죗죘죙죠죡죤죧죨죰죱죳죵죻주죽준줃줄줅줆줈줌줍줏중줗줘줙줜줟줠줨줩줫줬줭줴줵줸줻줼쥄쥅쥇쥈쥉쥐쥑쥔쥗쥘쥠쥡쥣쥥쥬쥭쥰쥴쥼쥽쥿즁즈즉즌즏즐즑즒즔즘즙즛증즞즤즥즨즬즴즵즷즹지직진짇질짊짏짐집짒짓짔징짖짗짙짚짛짜짝짞짠짢짣짤짧짨짬짭짯짰짱짷째짹짼짿쨀쨈쨉쨋쨌쨍쨔쨕쨘쨚쨜쨤쨥쨧쨩쨭쨰쨴쩌쩍쩐쩓쩔쩗쩘쩛쩜쩝쩟쩠쩡쩧쩨쩩쩬쩯쩰쩸쩹쩻쩼쩽쪁쪄쪅쪈쪋쪌쪔쪕쪗쪘쪙쪠쪡쪤쪨쪰쪱쪳쪵쪼쪽쪾쫀쫃쫄쫆쫌쫍쫏쫑쫒쫓쫗쫘쫙쫜쫟쫠쫨쫩쫫쫬쫭쫴쫵쫸쫻쫼쬈쬐쬑쬔쬗쬘쬠쬡쬣쬤쬥쬧쬬쬭쬰쬴쬼쬽쬿쭁쭈쭉쭊쭌쭏쭐쭒쭘쭙쭛쭝쭤쭥쭨쭫쭬쭴쭵쭷쭸쭹쮀쮁쮄쮇쮈쮐쮑쮓쮔쮕쮜쮝쮠쮣쮤쮬쮭쮯쮱쮸쮹쮼쯀쯈쯉쯋쯍쯔쯕쯘쯛쯜쯤쯥쯧쯩쯪쯰쯱쯴쯸찀찁찃찅찆찌찍찐찓찔찜찝찟찡찢찦찧차착찬찮찯찰찱참찹찻찼창찾찿챂채책챈챋챌챔챕챗챘챙챛챞챠챡챤챦챨챰챱챳챵챺챼첑처척천첟철첧첨첩첫첬청첮첲체첵첸첻첼쳃쳄쳅쳇쳈쳉쳐쳑쳔쳗쳘쳠쳡쳣쳤쳥쳬쳭쳰쳴쳼쳽쳿촁초촉촌촏촐촘촙촛총촤촥촨촫촬촴촵촷촹쵀쵄쵓쵔최쵝쵠쵣쵤쵬쵭쵯쵰쵱쵸쵹쵼춀춈춉춋춍추축춘춛출춤춥춧충춰춱춴춷춸췀췁췃췄췅췌췍췐췔췜췝췟췠췡취췩췬췯췰췸췹췻췽츄츅츈츌츔츕츗츙츠측츤츧츨츩츬츰츱츳층츼츽칀칄칌칍칏칑치칙친칟칠칡칢침칩칫칬칭칮칰칲카칵칸칻칼캄캅캇캈캉캍캎캐캑캔캗캘캠캡캣캤캥캩캪캬캭캰캴캼캽캿컁컈컌커컥컨컫컬컴컵컷컸컹컻컽컾케켁켄켇켈켐켑켓켔켕켙켚켜켝켠켣켤켬켭켯켰켱켸켹켼콀콈콉콋콌콍코콕콘콛콜콤콥콧콩콪콮콰콱콴콸쾀쾁쾃쾅쾌쾍쾐쾓쾔쾜쾝쾟쾡쾨쾩쾬쾰쾸쾹쾻쾽쿄쿅쿈쿌쿔쿕쿗쿙쿠쿡쿤쿧쿨쿰쿱쿳쿵쿺쿼쿽퀀퀃퀄퀌퀍퀐퀑퀘퀙퀜퀠퀨퀩퀫퀬퀭퀴퀵퀸퀻퀼큄큅큇큉큐큑큔큘큠큡큣큥큨크큭큰큲클큶큼큽큿킁킈킉킌킐킘킙킛킝키킥킨킫킬킴킵킷킸킹킾타탁탄탇탈탉탊탏탐탑탓탔탕탚태택탠탢탣탤탥탬탭탯탰탱탶탸탹탼턀턈턉턋턍턔턘턚터턱턴턷털턹턺턻텀텁텃텄텅테텍텐텓텔템텝텟텠텡텦텨텩텬텯텰텸텹텻텼텽톄톅톈톌톔톗톙토톡톤톧톨톰톱톳통톷톸톹톺톼톽퇀퇄퇌퇍퇏퇑퇘퇬퇭퇴퇵퇸퇻퇼툄툅툇툈툉툐툑툔툘툠툡툣툥투툭툰툳툴툶툼툽툿퉁퉈퉉퉌퉏퉐퉔퉘퉙퉛퉜퉝퉤퉥퉨퉫퉬퉴퉵퉷퉸퉹튀튁튄튇튈튐튑튓튕튜튝튠튤튬튭튯튱트특튻튼튿틀틁틂틄틈틉틋틍틔틕틘틜틠틤틥틧틩티틱틴틷틸팀팁팃팅팈팊파팍팎판팓팔팖팜팝팟팠팡팣팥팦패팩팬팯팰팸팹팻팼팽퍁퍄퍅퍈퍌퍔퍕퍗퍙퍠퍼퍽펀펃펄펌펍펎펏펐펑펕페펙펜펟펠펨펩펫펬펭펲펴펵편펹펻펼폄폅폇폈평폐폑폔폗폘폠폡폣폥포폭폮폰폳폴폼폽폿퐁퐄퐅퐆퐈퐉퐌퐐퐘퐙퐛퐝퐤푀푁푄푈푐푑푓푕표푝푠푣푤푬푭푯푱푸푹푼푿풀풂품풉풋풍풔풕풘풛풜풤풥풧풨풩풰풱풴풸퓀퓁퓃퓅퓌퓍퓐퓔퓜퓝퓟퓡퓨퓩퓬퓯퓰퓸퓹퓻퓽프픅픈픋플픐픔픕픗픙픠픡픤픨픰픱픳픵피픽핀핃필핌핍핏핑핕핖하학한핟할핡핣핤핥핧함합핫핬항핮핱해핵핸핻핼햄햅햇했행햍햐햑햔햘햠햡햣향햫햬헀허헉헌헏헐헑헒헕헗험헙헛헜헝헡헢헤헥헨헫헬헴헵헷헸헹헾혀혁현혇혈혐협혓혔형혜혝혠혤혬혭혯혰혱호혹혻혼혽혿홀홁홄홅홇홈홉홋홍홑화확환홛활홠홤홥홦홧홨황홰홱홴홷홸횀횁횃횄횅회획횐횓횔횜횝횟횡효횩횬횯횰횸횹횻횽후훅훈훋훌훍훎훑훒훓훔훕훗훙훜훝훠훡훤훨훰훱훳훴훵훼훽휀휃휄휌휍휏휐휑휘휙휜휟휠휨휩휫휭휴휵휸휺휻휼흃흄흅흇흉흍흐흑흔흕흖흗흘흙흝흟흠흡흣흥흨흩흪흫희흭흰흳흴흼흽흿힁히힉힌힏힐힗힘힙힛힜힝힠"""
	return opt, config
