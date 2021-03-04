class AFND{
	private:
		Alphabet alphabet;
		int initialState;
		vector<int> States;
		vector<int> finalStates;
		vector<int> ActualStates;
		vector<vector<vector<int>>> transitionFunction;
		bool Lambda;
		vector<vector<vector<int>>> convertTransitionFunction(string,int,int,bool);
		vector<vector<vector<int>>> convertTransitionFunction(string,int,int,string,bool);
		int getSeparatorSize();
		string stringSeparator(int,int);
		Alphabet getAlphabetFile(string);
		string alphabetSeparator(int);
	public:
		AFND();
		AFND(Alphabet,int,int,vector<int>,vector<vector<vector<int>>>,bool);
		AFND(string,int,int,string,string,bool);
		AFND(string);
		AFND(RegularLenguage);
		~AFND();
		Alphabet getAlphabet();
		int getInitialState();
		vector<int> getStates();
		vector<int> getFinalStates();
		vector<int> getActualStates();
		vector<vector<vector<int>>> getTransitionFunction();
		bool getLambda();
		void setAlphabet(Alphabet);
		void setInitialState(int);
		void setStates(vector<int>);
		void setStates(int);
		void setActualStates(vector<int>);
		void setTransitionFunction(vector<vector<vector<int>>>);
		void setLambda(bool);
		void print();
		vector<int> getClosure(int);
		void transition(char);
		bool evaluation(string);
		void restart();
		static vector<vector<vector<int>>> makeEmptyTransition(int,int);
};
AFND::AFND(){
	alphabet=Alphabet();
}
AFND::AFND(Alphabet _alphabet,int numberOfStates,int _initialState,vector<int> _finalStates,
	vector<vector<vector<int>>> _transitionFunction,bool _Lambda){
	alphabet=_alphabet;
	setStates(numberOfStates);
	initialState=_initialState;
	ActualStates=vector<int>(1,initialState);
	finalStates=_finalStates;
	transitionFunction=_transitionFunction;
	Lambda=_Lambda;
}
AFND::AFND(string _alphabet,int numberOfStates,int _initialState,string _finalStates,
	string _transitionFunction,bool _Lambda){
	alphabet=Alphabet(_alphabet);
	setStates(numberOfStates);
	initialState=_initialState;
	ActualStates=vector<int>(1,initialState);
	finalStates=getStringNumbers(_finalStates);
	Lambda=_Lambda;
	transitionFunction=convertTransitionFunction(_transitionFunction,alphabet.getChars().size(),
		numberOfStates,_Lambda);
	printVectorInt(getActualStates(),"A","","\n");
}
Alphabet AFND::getAlphabetFile(string fileInput){
	ifstream fi;
	fi.open(fileInput,ios::in);
	Alphabet _Alphabet;
	if(!fi.fail()){
		string alphabetType;
		for(int i=0;i<4;i++){
			getline(fi,alphabetType);
		}
		if(alphabetType[0]=='d'){
			_Alphabet=Alphabet();
		}else if(alphabetType[0]=='s'){
			string _alphabet;
			getline(fi,_alphabet);
			_Alphabet=Alphabet(_alphabet);
		}else if(alphabetType[0]=='i'){
			int first,last;
			fi>>first;
			fi>>last;
			_Alphabet=Alphabet(first,last);
		}else if(alphabetType[0]=='c'){
			char first,last;
			fi>>first;
			fi>>last;
			_Alphabet=Alphabet(first,last);
		}
	}else{
		cerr<<"File afd error"<<endl;
		exit(1);
	}
	fi.close();
	return _Alphabet;
}
AFND::AFND(string fileAFND){
	ifstream fileInput;
	fileInput.open(fileAFND,ios::in);
	if(!fileInput.fail()){
		string _Lambda,_consider,_chars,_numberOfStates,_initialState,_finalStates
			,aux(""),_transitionFunction(""),alphabetType;
		getline(fileInput,_Lambda);
		getline(fileInput,_consider);
		getline(fileInput,_chars);
		alphabet = getAlphabetFile(fileAFND);
		getline(fileInput,alphabetType);
		if(alphabetType=="i"||alphabetType=="c"){
			getline(fileInput,aux);
			getline(fileInput,aux);
		}else if(alphabetType=="s"){
			getline(fileInput,aux);
		}
		getline(fileInput,_numberOfStates);
		getline(fileInput,_initialState);
		getline(fileInput,_finalStates);
		while(!fileInput.eof()){
			getline(fileInput,aux);
			_transitionFunction+=aux;
		}
		setStates(atoi(_numberOfStates.c_str()));
		initialState=atoi(_initialState.c_str());
		finalStates=getStringNumbers(_finalStates);
		if(_Lambda[2]=='t'){
			setLambda(true);
		}else{
			setLambda(false);
		}
		if(_consider[2]=='f'){
			transitionFunction=convertTransitionFunction(_transitionFunction,alphabet.getChars().size(),
			atoi(_numberOfStates.c_str()),Lambda);
		}else{
			transitionFunction=convertTransitionFunction(_transitionFunction,alphabet.getChars().size(),
			atoi(_numberOfStates.c_str()),_chars,Lambda);	
		}
	}else{
		cerr<<"File afnd error"<<endl;
		exit(1);
	}
}
AFND::AFND(RegularLenguage rl){
	int count=0,_actualState=0,numberOfStates=rl.getWords().size(),character=0;
	alphabet = rl.onlyUsedAlphabet();
	vector<int> _final = vector<int>(0);
	vector<vector<vector<int>>> transition = makeEmptyTransition(1,alphabet.getChars().size());
	vector<vector<int>> auxRow = transition[0];
	for(int i = 0; i<numberOfStates; i++){
		string actualword = rl.getWords()[i];
		for(int j = 0; j<actualword.size(); j++){
			character=alphabet.getChars().find(actualword[j]);
			if(transition[_actualState][character].size()==0){
				transition[_actualState][character].push_back((count+1));
				count++;
				_actualState = count;
				transition.push_back(auxRow);
			}else if(j==actualword.size()-1){
					_final.push_back(transition[_actualState][character][0]);
			}else{
				_actualState=transition[_actualState][character][0];
			}
		}
		_final.push_back(count);
		_actualState=0;
	}
	finalStates=_final;
	setStates(count+1);
	initialState=0;
	transitionFunction=transition;
	Lambda=false;
}
AFND::~AFND(){}
Alphabet AFND::getAlphabet(){
	return alphabet;
}
int AFND::getInitialState(){
	return initialState;
}
vector<int> AFND::getStates(){
	return States;
}
vector<int> AFND::getFinalStates(){
	return finalStates;
}
vector<int> AFND::getActualStates(){
	return ActualStates;
}
vector<vector<vector<int>>> AFND::getTransitionFunction(){
	return transitionFunction;
}
bool AFND::getLambda(){
	return Lambda;
}
void AFND::setAlphabet(Alphabet _alphabet){
	alphabet=_alphabet;
}
void AFND::setInitialState(int _initialState){
	initialState=_initialState;
}
void AFND::setStates(vector<int> _states){
	States=_states;
}
void AFND::setStates(int numberOfStates){
	States = vector<int>();
	for(int i=0;i<numberOfStates;i++){
		States.push_back(i);
	}
}
void AFND::setActualStates(vector<int> _actualStates){
	ActualStates=_actualStates;
}
void AFND::setTransitionFunction(vector<vector<vector<int>>> _transitionFunction){
	transitionFunction=_transitionFunction;
}
void AFND::setLambda(bool _Lambda){
	Lambda=_Lambda;
}
vector<vector<vector<int>>> AFND::convertTransitionFunction(string _transitionFunction,int chars,
	int _states,bool _Lambda){
	int counter = 0;
	vector<vector<vector<int>>> Transition;
	vector<string> splitTransition = splitString(_transitionFunction,"{");
	vector<int> _numbers;
	vector<vector<int>> rowNumbers;
	if(_Lambda){
		chars++;
	}
	for(int i=0;i<_states;i++){
		for(int j=0;j<chars;j++){
			_numbers=getStringNumbers(splitTransition[counter]);
			rowNumbers.push_back(_numbers);
			counter++;
		}
		Transition.push_back(rowNumbers);
		rowNumbers.clear();
	}
	return Transition;
}
vector<vector<vector<int>>> AFND::convertTransitionFunction(string _transitionFunction,int chars,
	int _states,string consider,bool _Lambda){
	int counter = 0,alphabetSize=chars,lambda=1,cs=consider.size();
	vector<vector<vector<int>>> Transition;
	vector<string> splitTransition = splitString(_transitionFunction,"{");
	vector<int> _numbers;
	vector<vector<int>> rowNumbers;
	if(_Lambda){
		chars++;
		lambda++;
	}
	for(int i=0;i<_states;i++){
		for(int j=0;j<chars;j++){
			if(j<alphabetSize){
				int found=consider.find(alphabet.getChars()[j]);
				if(found!=string::npos){
					_numbers=getStringNumbers(splitTransition[found+(cs+lambda)*i]);
				}else{
					_numbers=getStringNumbers(splitTransition[(cs+lambda)*i+cs]);
				}
			}else{
				_numbers=getStringNumbers(splitTransition[(cs+2)*i+cs+1]);
			}
			rowNumbers.push_back(_numbers);
		}
		Transition.push_back(rowNumbers);
		rowNumbers.clear();
	}
	return Transition;
}
int AFND::getSeparatorSize(){
	int separator = 8, chars=alphabet.getChars().size(),aux=0;
	if(Lambda){
		chars++;
	}
	for(int i=0;i<States.size();i++){
		for(int j=0;j<chars;j++){
			aux=(transitionFunction[i][j].size()*2)+2;
			if(aux>separator){
				separator=aux;
			}
		}
	}
	return separator;
}
string AFND::stringSeparator(int _state,int _char){
	string separator("");
	int sep = getSeparatorSize(),aux1;
	if(Lambda&&_char==alphabet.getChars().size()){
		return separator;
	}else if(!Lambda&&_char==alphabet.getChars().size()-1){
		return separator;
	}else{
		aux1=(transitionFunction[_state][_char].size()*2)+1;
		if(aux1<2){
			aux1=2;
		}
		for(int i=0;i<sep-aux1;i++){
			separator+=" ";
		}
	}
	return separator;
}
string AFND::alphabetSeparator(int _char){
	string separator("");
	int sep=stringSeparator(0,_char).size(),aux;
	for(int i=0;i<States.size();i++){
		aux=stringSeparator(i,_char).size();
		if(aux>sep){
			sep=aux;
		}
	}
	for(int i=0;i<sep+1;i++){
		separator+=" ";
	}
	return separator;
}
void AFND::print(){
	cout<<"\t|";
	string sep("");
	int separator = getSeparatorSize(),chars=alphabet.getChars().size(),lines;
	if(Lambda){
		chars++;
	}
	lines=chars+1;
	for(int i=0;i<separator;i++){
		sep+=" ";
	}
	for(int j=0;j<alphabet.getChars().size();j++){
		cout<<alphabet.getChars()[j]<<alphabetSeparator(j);
	}
	cout<<endl;
	for(int i=0;i<lines*separator;i++){
		cout<<"-";
	}
	cout<<endl;
	for(int i=0;i<States.size();i++){
		if(States[i]==initialState&&containsVectorInt(finalStates,States[i])){
			cout<<"*->q"<<States[i]<<"\t|";
		}else if(States[i]==initialState){
			cout<<"->q"<<States[i]<<"\t|";
		}else if(containsVectorInt(finalStates,States[i])){
			cout<<"*q"<<States[i]<<"\t|";
		}else{
			cout<<"q"<<States[i]<<"\t|";
		}
		for(int j=0;j<chars;j++){
			printVectorInt(transitionFunction[i][j],"{",",","}");
			cout<<stringSeparator(i,j);
		}
		cout<<endl;
	}
}
vector<int> AFND::getClosure(int _state){
	vector<int> Closure = vector<int>(1,_state);
	if(Lambda){
		int _char = alphabet.getChars().size(),size1=1,size2;
		vector<int> _Lambda = transitionFunction[_state][_char];
		for(int i=0;i<_Lambda.size();i++){
			if(!containsVectorInt(Closure,_Lambda[i])){
				Closure.push_back(_Lambda[i]);
			}
		}
		size2=Closure.size();
		while(size1!=size2){
			for(int i=size1;i<size2;i++){
				_Lambda=transitionFunction[Closure[i]][_char];
				for(int j=0;j<_Lambda.size();j++){
					if(!containsVectorInt(Closure,_Lambda[j])){
						Closure.push_back(_Lambda[j]);
					}
				}
			}
			size1=size2;
			size2=Closure.size();
		}
	}
	return Closure;
}
void AFND::transition(char character){
	int p = alphabet.getChars().find_first_of(character);
	if(p!=string::npos){
		vector<int> CL = vector<int>(0);
		for(int i=0;i<ActualStates.size();i++){
			CL=mergeVectorInt(CL,getClosure(ActualStates[i]));
		}
		ActualStates=vector<int>(0);
		for(int i=0;i<CL.size();i++){
			ActualStates=mergeVectorInt(ActualStates,transitionFunction[CL[i]][p]);
		}
	}
}
bool AFND::evaluation(string cad){
	ActualStates=vector<int>(1,initialState);
	if(alphabet.isWord(cad)){
		for(int i=0;i<cad.size();i++){
			transition(cad[i]);
		}
		for(int i=0;i<ActualStates.size();i++){
			vector<int> CL = getClosure(ActualStates[i]);
			for(int j=0;j<CL.size();j++){
				if(containsVectorInt(finalStates,CL[j])){
					return true;
				}
			}
		}
		return false;
	}else return false;
}
void AFND::restart(){
	ActualStates=vector<int>(1,initialState);
}
vector<vector<vector<int>>> AFND::makeEmptyTransition(int _states,int _chars){
	vector<vector<vector<int>>> Transition;
	vector<vector<int>> rowNumbers;
	vector<int> _numbers=vector<int>(0);
	for(int i=0;i<_states;i++){
		for(int j=0;j<_chars;j++){
			rowNumbers.push_back(_numbers);
		}
		Transition.push_back(rowNumbers);
		rowNumbers.clear();
	}
	return Transition;
}