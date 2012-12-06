#include "pbUI.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ �ֹ��� UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbGauge_Indicator::pbGauge_Indicator(){
	m_fMaxGaugePoint = 0;
	m_bGaugeChangeStart = false;

	 m_vPos[0] = 0.0f;
	 m_vPos[1] = 0.0f;

	m_fDestPercent = 0;
	m_fStartPercent = 0;
	m_fDrawPercent = 0.0f;
	m_fDrawPosX = 0.0f;

	m_fAniTime = 0.0f;

	m_fGaugeHalfWidth = 0;
	m_fGaugeHalfHeight = 0;

	m_GaugeUV_WidthPercent = 1.0f;

	m_GaugeUVBindID = 0;

	m_pBaseDrawUnit = new pbBasicDrawUnit();

	m_fpGaugeReturnFunc = NULL;
	m_pV2RelativePos = NULL;
}

pbGauge_Indicator::~pbGauge_Indicator()
{
	delete m_pBaseDrawUnit;

}


void pbGauge_Indicator::SetPos(float PosX, float PosY){
	m_vPos[0] = PosX;
	m_vPos[1] = PosY;

	//m_fDrawPosX = m_fGaugeHalfWidth*(1.0f - m_fDrawPercent );
}

void pbGauge_Indicator::SetRelativePos(float* pV2Pos){
	m_pV2RelativePos = pV2Pos;
}

void pbGauge_Indicator::SetBaseSprite(screenplayTag Tag, float Width, float Height)
{
	m_pBaseDrawUnit->SetTextureTAG(Tag);
	m_pBaseDrawUnit->SetSize(Width, Height);

	//SetPos(0,0);
}

void pbGauge_Indicator::SetGaugeSprite(screenplayTag Tag, float Width, float Height){
	//크기 설정
	SetVertexByCenter(m_GaugeVertex, Width, Height);
	m_fGaugeHalfWidth =  Width/2;
	m_fGaugeHalfHeight =  Height/2;

	//UV 추출
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(Tag);

	int index = pSprite->currentScreenplay->getKernel();
	TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(index);
	UVPacket* uvPacket = &textureAtlasIterator->second;

	m_GaugeUVBindID = uvPacket->bindTextureID;
	memcpy(m_GaugeUV, uvPacket->texture, sizeof(GLfloat)*8);

	m_GaugeUV_WidthPercent = m_GaugeUV[4] - m_GaugeUV[0];
}
void pbGauge_Indicator::PreSettingDraw(){
	glPushMatrix();
	if( m_pV2RelativePos != NULL)
		glTranslatef(m_pV2RelativePos[0], m_pV2RelativePos[1],  0);
		glTranslatef(m_vPos[0], m_vPos[1],  0);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbGauge_Indicator::DrawThis() {
		///------------Power Text--------------------
		glPushMatrix();
		m_pBaseDrawUnit->PreSettingDraw();
		m_pBaseDrawUnit->DrawThis();
		glPopMatrix();
		///------------게이지 텍스쳐 바인드--------------------
		glBindTexture(GL_TEXTURE_2D,  m_GaugeUVBindID );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glVertexPointer(3, GL_FLOAT, 0, m_GaugeVertex);
		glTexCoordPointer(2,GL_FLOAT, 0,  m_GaugeUV);	//�ؽ�ó��ǥ(UV) �迭 �Ѱ��ֱ�

		glPushMatrix();
		m_GaugeUV[4] = m_GaugeUV[0] + m_GaugeUV_WidthPercent*m_fDrawPercent;
		m_GaugeUV[6] = m_GaugeUV[2] + m_GaugeUV_WidthPercent*m_fDrawPercent;
		glTranslatef( m_fDrawPosX, 0 ,  0);
		glScalef(m_fDrawPercent, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


void pbGauge_Indicator::Update(float fTime)
{
	if( m_fpGaugeReturnFunc != NULL)
	{
		float NowGaugePoint = (*m_fpGaugeReturnFunc)();
		if( NowGaugePoint != -1.0f)
		{
			m_fDestPercent = NowGaugePoint/m_fMaxGaugePoint;
			m_fStartPercent = m_fDrawPercent;
			m_bGaugeChangeStart = true;
			m_fAniTime = 0.0f;
		}
	}


	if(m_bGaugeChangeStart) {
			m_fAniTime += fTime;

			m_fDrawPercent = npLerp(m_fStartPercent, m_fDestPercent, m_fAniTime);
			m_fDrawPosX = - m_fGaugeHalfWidth*(1.0f - m_fDrawPercent);
			if(m_fAniTime > 1.0f) {
				m_fAniTime = 0.0f;
				m_fDrawPercent = m_fDestPercent;

				m_bGaugeChangeStart = false;
			}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���� UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbBackPanelUI::pbBackPanelUI(){
	m_pBaseDrawUnit = new pbBasicDrawUnit();

	 m_vPos[0] = 0.0f;
	 m_vPos[1] = 0.0f;
}

pbBackPanelUI::~pbBackPanelUI(){
	delete m_pBaseDrawUnit;

}


void pbBackPanelUI::SetPos(float PosX, float PosY)
{
	m_vPos[0] = PosX;
	m_vPos[1] = PosY;

	//m_TouchArea.movePositon(PosX, PosY);
}

void pbBackPanelUI::SetBaseSprite(screenplayTag Tag, float Width, float Height){
	m_pBaseDrawUnit->SetTextureTAG(Tag);
	m_pBaseDrawUnit->SetSize(Width, Height);

	//SetPos(0,0);
}

void pbBackPanelUI::PreSettingDraw(){
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	m_pBaseDrawUnit->PreSettingDraw();
}

void pbBackPanelUI::DrawThis() {
	m_pBaseDrawUnit->DrawThis();

	glPopMatrix();
}

void pbBackPanelUI::Update(float fTime)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���� UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbButtonUI::pbButtonUI(){
	m_pBaseDrawUnit = new pbBasicDrawUnit();
}

pbButtonUI::~pbButtonUI(){
	delete m_pBaseDrawUnit;
}
void pbButtonUI::SetPos(float PosX, float PosY){
	m_vPos[0] = PosX;
	m_vPos[1] = PosY;

}

void pbButtonUI::SetBaseSprite(screenplayTag Tag, float Width, float Height){
	m_pBaseDrawUnit->SetTextureTAG(Tag);
	m_pBaseDrawUnit->SetSize(Width, Height);

}

void pbButtonUI::PreSettingDraw(){
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	m_pBaseDrawUnit->PreSettingDraw();
}

void pbButtonUI::DrawThis() {
	m_pBaseDrawUnit->DrawThis();
	glPopMatrix();
}

void pbButtonUI::Update(float fTime)
{

}
void pbButtonUI::notify() {
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
	//	LOGE("[DEBUG]pbTouchableBackground:: TAPDOWN");

		int x = TouchLayer::GetInstance().pointX;
		int y = TouchLayer::GetInstance().pointY;
//		LOGfloatString("X", x);
//		LOGfloatString("Y", y);

		int HalfWidth = m_pBaseDrawUnit->getWidth()/2;
		int HalfHeight =m_pBaseDrawUnit->getHeight()/2;

		int left = m_vPos[0] - HalfWidth;
		int right = m_vPos[0] + HalfWidth;
		int top = m_vPos[1] + HalfHeight;
		int bottom = m_vPos[1] - HalfHeight;

		if (x >= left && x <= right) {
			if (y >= bottom && y <= top) {
				if( m_fpTouchFunc != NULL)
					(*m_fpTouchFunc)();
				LOGE("[DEBUG]pbMenuButtonUI:: Touched");
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- ���ھ� ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbNumber_Indicator::pbNumber_Indicator(){
	m_pBaseDrawUnit = new pbBasicDrawUnit();

	m_NumberWidth = 0;

	m_fTextHalfWidth = 0;

	m_CurrentDigits = 1;

	m_fpNumberReturnFunc = NULL;
}

pbNumber_Indicator::~pbNumber_Indicator(){
	delete m_pBaseDrawUnit;
}

void pbNumber_Indicator::DataReset(){
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;
}

void pbNumber_Indicator::SetBaseSprite(screenplayTag Tag, float Width, float Height) {
	m_pBaseDrawUnit->SetTextureTAG(Tag);
	m_pBaseDrawUnit->SetSize(Width, Height);
	m_fTextHalfWidth =   Width/2;
}

void pbNumber_Indicator::SetScoreSprite(screenplayTag ZeroSpriteTag, float Width, float Height) {
	//크기 설정
	SetVertexByCenter(m_NumberVertex, Width, Height);
	m_NumberWidth = Width;

	//UV 추출
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(ZeroSpriteTag);

	int index;
	for(int i = 0; i < NUMBERING; i++)
	{
		index = pSprite->currentScreenplay->getKernel();
		TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(index);
		UVPacket* uvPacket = &textureAtlasIterator->second;

		m_NumberUV[i] = uvPacket;

		pSprite->ReadyForNextScreenplay();
	}
}

void pbNumber_Indicator::SetShowDigits(int ShowDigits) {
	if( ShowDigits < MAX_DIGITS )
		m_CurrentDigits = ShowDigits;
	else {
		m_CurrentDigits = MAX_DIGITS;
		LOGE("pbScore_Indicator:: Value Range Over : MAX_DIGITS");
	}
}

void pbNumber_Indicator::SetNumberReturnFunc(int(Func)() ) {
	m_fpNumberReturnFunc = Func;
}

void pbNumber_Indicator::PreSettingDraw() {
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0);
}

void pbNumber_Indicator::DrawThis() {
		glPushMatrix();
		///---------text----------------//
		m_pBaseDrawUnit->PreSettingDraw();
		glTranslatef( -m_fTextHalfWidth - 10,  0.0f,  0);
		m_pBaseDrawUnit->DrawThis();
		glPopMatrix();

		glVertexPointer(3, GL_FLOAT, 0, m_NumberVertex);

		///----------score------------------//
		for (int i = 0; i < m_CurrentDigits; i++) {
			glPushMatrix();
			UVPacket* UV = m_NumberUV[m_DigitsNumber[i]];
			glBindTexture(GL_TEXTURE_2D,   UV->bindTextureID);
			glTexCoordPointer(2,GL_FLOAT, 0,  UV->texture);
			glTranslatef(8 + ((float) i) * m_NumberWidth, 0, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();
		}
	glPopMatrix();
}



void pbNumber_Indicator::Update(float fTime)
{
	int ReturnNumber = (*m_fpNumberReturnFunc)();
	if( ReturnNumber != -1 )	{
		SetNumber(ReturnNumber);
	}
}

void pbNumber_Indicator::SetNumber(int Number) {
	int TempScore = Number;
	for( int i = m_CurrentDigits - 1; i > 0; --i)
	{
		m_DigitsNumber[i] = TempScore%10;
		TempScore /= 10;
	}

/*		LOGfloatString("Real", m_NumberData);
	LOGfloatString("1", m_DigitsNumber[0]);
	LOGfloatString("10", m_DigitsNumber[1]);
	LOGfloatString("100", m_DigitsNumber[2]);
	LOGfloatString("1000", m_DigitsNumber[3]);
	LOGfloatString("10000", m_DigitsNumber[4]);
	LOGfloatString("100000", m_DigitsNumber[5]);*/
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------XMLParsingBufferCreater ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
XMLParsingBufferCreater::XMLParsingBufferCreater() {
	this->apkArchive = NULL;
}
XMLParsingBufferCreater::~XMLParsingBufferCreater() {
	if( apkArchive != NULL)
		zip_close(apkArchive);
}

void XMLParsingBufferCreater::SetArchive(std::string apkPath) {
	LOGE("XMLparser)setup APk path : %s",apkPath.c_str());

	this->apkArchive = zip_open(apkPath.c_str(),0,NULL);

	if(this->apkArchive == NULL){
		LOGE("npXMLParsing) Error loading APk");
	}
}

char* XMLParsingBufferCreater::OpenAssetsByPath(xmlPath path) {
	if( apkArchive != NULL) {
		char filePath[128];
		sprintf(filePath,"assets/%s\0",path.c_str());

		LOGE("png Path: %s",filePath);
		zip_file* apkZipFile = zip_fopen(this->apkArchive,filePath,0);
		if(NP_IS_EMPTY(apkZipFile)){
			LOGE("XMLparser) not open zipFile");
			return NULL;
		}
		LOGE("done zip fopen");
		LOGE("zip Left Bytes: %d",apkZipFile->bytes_left);

		char* buffer= new char[apkZipFile->bytes_left];
		zip_fread(apkZipFile,buffer,apkZipFile->bytes_left);
		zip_fclose(apkZipFile);

		return buffer;
	}

	return NULL;
}
void XMLParsingBufferCreater::CloseAssetsAndBuffer(char* buffer) {
	if( buffer != NULL) {
		delete buffer;
	}

}

XMLParsingBufferCreater& XMLParsingBufferCreater::GetInstance() {
	static XMLParsingBufferCreater Singleton;
	return Singleton;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbUIProcessor------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbUIProcessor* pbUIProcessor::SingleObject = NULL;
pbUIProcessor::pbUIProcessor():m_UICounts(0),m_ControledUIStore(NULL)/*,m_UIStore(NULL)*/{

}

pbUIProcessor::~pbUIProcessor(){

}

void pbUIProcessor::Create()
{
	//---------------------------���μ��� �ʱ�ȭ-----------------------------------//
	if(SingleObject == NULL){
		SingleObject = new pbUIProcessor;
		//��Ʈ ���μ����� Store�� List�� Head�� �����Ѵ�.
		SingleObject->m_ControledUIStore = new UIList;
		SingleObject->m_ControledUIStore->setHeader();

		LOGI("UIProcessor Create Complete");
	}


	LOGI("pbUIProcesser Initialized");
}

bool pbUIProcessor::LoadData(const char* filename) {
/*	//------------XML �Ľ�--------//
		char* buffer = XMLParsingBufferCreater::GetInstance().OpenAssetsByPath(filename);
		if( buffer == NULL) return false;

		TiXmlDocument xmlDoc;
		xmlDoc.Parse(buffer);

		if(xmlDoc.Error()){
			LOGE("XmlParser error");
		}

			//-----------------�ؽ��ĵ����� ��� �Ľ�----------------------//
			TiXmlNode* DataHeadNode = xmlDoc.FirstChild("UI_Placement")->FirstChild("UIDataHeader");

			TiXmlNode* UIData = DataHeadNode->FirstChild("UIData");

			while( UIData != NULL) {
				const char* Type = UIData->ToElement()->GetText();
				int ID;
				double PosX, PosY;
				const char* Tag1, *Tag2;

				UIData->ToElement()->Attribute("ID", &ID);
				UIData->ToElement()->Attribute("X", &PosX);
				UIData->ToElement()->Attribute("Y", &PosY);
				Tag1 = UIData->ToElement()->Attribute("TAG_1");
				Tag2 = UIData->ToElement()->Attribute("TAG_2");

				SingleObject->CreateUIByType(Type, PosX, PosY, Tag1, Tag2);

				UIData = UIData->NextSibling();
			}

		LOGI("Done UI Xml parsing");

		xmlDoc.Clear();
		XMLParsingBufferCreater::GetInstance().CloseAssetsAndBuffer(buffer);*/

		return true;
}

/*void pbUIProcessor::CreateUIByType(const char* Type, float X, float Y, screenplayTag BaseTag, screenplayTag ExtraTag) {
	if( strcmp(Type, "BackPanel") == 0 ) {
		AddBackPanelUI(X, Y, BaseTag);
	}
	else if( strcmp(Type, "Menu") == 0 ) {
		AddButtonUI(X, Y, BaseTag);
	}
	else if( strcmp(Type, "Help") == 0 ) {
		AddButtonUI(X, Y, BaseTag);
	}
	else if( strcmp(Type, "APGauge") == 0 ) {
		AddAbillityPointUI(X, Y, BaseTag, ExtraTag);
	}
	else if( strcmp(Type, "Score") == 0 ) {
		AddScoreUI(X, Y, BaseTag, ExtraTag);
	}
}*/

//----------------------------BackPanel-------------------------------------//
pbBasicUI* pbUIProcessor::AddBackPanelUI(float X, float Y,  screenplayTag Tag, float Width, float Height) {
	pbBackPanelUI* newUI = new pbBackPanelUI();
	newUI->SetPos(X, Y);
	newUI->SetBaseSprite(Tag, Width, Height);

	//newUI ->SetID(ID);   // ID������ ����� �غ��� ��
	registControled(newUI );


	return newUI;
}

//----------------------------Menu-------------------------------------//
pbTouchUI* pbUIProcessor::AddButtonUI(float X, float Y,  screenplayTag Tag, float Width, float Height, void(Func)()) {
	pbTouchUI* newUI = new pbButtonUI();
	newUI->SetPos(X, Y);
	newUI->SetBaseSprite(Tag, Width, Height);
	newUI->SetTouchFunc(Func);

	registControled(newUI );
	TouchLayer::GetInstance().RegistedObserver(newUI);

	return newUI;
}

//----------------------------Score-------------------------------------//
pbNumber_Indicator* pbUIProcessor::AddNumberUI(float X, float Y, screenplayTag TextTag, float TextWidth, float TextHeight,
																				screenplayTag ZeroNumberTag, float NumberWidth, float NumberHeight,
																				int MaxDigits, int(NumberRetunFunc)())
{
	pbNumber_Indicator* newUI = new pbNumber_Indicator();
	newUI->SetPos(X, Y);
	newUI->SetBaseSprite(TextTag, TextWidth, TextHeight);
	newUI->SetScoreSprite(ZeroNumberTag, NumberWidth, NumberHeight);
	newUI->DataReset();
	newUI->SetShowDigits(MaxDigits);
	newUI->SetNumberReturnFunc(NumberRetunFunc);

	registControled(newUI );
	return newUI;

}
//----------------------------APGauge-------------------------------------//
pbBasicUI* pbUIProcessor::AddGaugeUI(float X, float Y, screenplayTag GaugePanelTag,  screenplayTag GaugeTag, float GaugeWidth, float GaugeHeight,  float MaxGauge, float(GaugeReturnFunc)()) {
	pbGauge_Indicator* newUI = new pbGauge_Indicator();
	newUI->SetBaseSprite(GaugePanelTag, GaugeWidth, GaugeHeight);
	newUI->SetGaugeSprite(GaugeTag, GaugeWidth, GaugeHeight);
	newUI->SetGaugeReturnFunc(GaugeReturnFunc);
	newUI->SetMaxGaugePoint(MaxGauge);
	newUI->SetPos(X, Y);

	registControled(newUI );

	return newUI;
}
pbBasicUI* pbUIProcessor::AddGaugeUI_RelativePos(float* pV2RelativePos, float X, float Y, screenplayTag GaugePanelTag,  screenplayTag GaugeTag, float GaugeWidth, float GaugeHeight,  float MaxGauge, float(GaugeReturnFunc)()) {
	pbGauge_Indicator* newUI = new pbGauge_Indicator();
	newUI->SetBaseSprite(GaugePanelTag, GaugeWidth, GaugeHeight);
	newUI->SetGaugeSprite(GaugeTag, GaugeWidth, GaugeHeight);
	newUI->SetGaugeReturnFunc(GaugeReturnFunc);
	newUI->SetMaxGaugePoint(MaxGauge);
	newUI->SetPos(X, Y);
	newUI->SetRelativePos(pV2RelativePos);

	registControled(newUI );

	return newUI;
}


void pbUIProcessor::Update(float time){
	UIList* iterator;
	UIList* head = m_ControledUIStore;

	iterator = head->getNext();
	while( iterator != head ) {
		pbUI* pUI = iterator->getKernel();
		iterator = iterator->getNext();

		pUI->Update(time);
	}

}

void pbUIProcessor::ClearDataStore() {
	LinkNodeDeleteAllKernel(pbUI*, m_ControledUIStore)
	UIList::clearList(m_ControledUIStore);

	LOGfloatString("ClearUI :", m_UICounts);

	m_UICounts = 0;
	LOGI("pbUIProcesser ClearDataStore");
}

void pbUIProcessor::Release(){
	if( SingleObject != NULL) {
		SingleObject->ClearDataStore();
		UIList::destroyList(SingleObject->m_ControledUIStore);

		delete SingleObject;
		LOGI("pbUIProcesser Release");

		SingleObject = NULL;
	}
}

void pbUIProcessor::registControled(pbUI* pUI){
	UIList::addTail(pUI,m_ControledUIStore);

	m_UICounts++;
}

void pbUIProcessor::removeControled(pbUI* pUI){
	UIList::findDelete(pUI,m_ControledUIStore);
}

void pbUIProcessor::DeleteUI(pbUI* pUI) {
	if( pUI != NULL ) {
		removeControled(pUI);
		delete pUI;

		m_UICounts--;
	}
}
