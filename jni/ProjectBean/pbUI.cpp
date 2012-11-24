#include "pbUI.h"
#include "pbMainFrame.h"
#include "tinyxml.h"


////----------------------------------------------------- �������̽�  UI------------------------------------------------------------------------------///////
void pbInterfaceUI::registLinkNode(pbInterfaceUI* pUI){
	pbUIList* pTargetNode =  pbUIList::makeLinkNode(pUI);
	pbUIList::addTail(pTargetNode, m_pUIListHeader);
}

void pbInterfaceUI::removeLinkNode(pbInterfaceUI* pUI){
	pbUIList* pTargetNode =  pbUIList::makeLinkNode(pUI);
	pbUIList::findDelete(pTargetNode, m_pUIListHeader);
}

pbInterfaceUI::pbInterfaceUI(){
	m_Type = UITYPE::UI;
	m_vPos[iX] = 0;
	m_vPos[iY] = 0;
	m_vParentPos[iX] = 0;
	m_vParentPos[iY] = 0;

	m_pDrawUnit_Base = new pbDrawUnit;
	m_pUIListHeader = NULL;
}
pbInterfaceUI::~pbInterfaceUI() {
	delete m_pDrawUnit_Base;

	if( m_pUIListHeader != NULL ) {
		pbUIList::destroyListAndDeleteKernel(m_pUIListHeader);
	}
};

void pbInterfaceUI::AddChildUnit(pbInterfaceUI* pChild) {
	if( m_pUIListHeader == NULL ) {
		m_pUIListHeader = new pbUIList;
		m_pUIListHeader->setHeader();
	}

	registLinkNode(pChild);
}
void pbInterfaceUI::DeleteChildUnit(pbInterfaceUI* pChild) {
	if( m_pUIListHeader != NULL ) {
		pbUIList* Iter = m_pUIListHeader->getNext();

		while(Iter != m_pUIListHeader ) {
			pbInterfaceUI* pUI = Iter->getKernel();
			Iter = Iter->getNext();

			if( pUI == pChild) {
				removeLinkNode(pUI);
				delete pUI;
			}

		}
	}
}

void pbInterfaceUI::Draw(){
	glPushMatrix();

	glTranslatef(m_vPos[iX], m_vPos[iY],  0);

	GetBaseDrawUnit()->Draw();

	if( m_pUIListHeader != NULL ) {
		pbUIList* Iter = m_pUIListHeader->getNext();

		while(Iter != m_pUIListHeader ) {
			pbInterfaceUI* pUI = Iter->getKernel();
			pUI->Draw();
			Iter = Iter->getNext();
		}
	}

	glPopMatrix();
}

void pbInterfaceUI::SetV2Pos(float X, float Y) {
	m_vPos[iX] = X; m_vPos[iY] = Y;

	if( m_pUIListHeader != NULL ) {
		pbUIList* Iter = m_pUIListHeader->getNext();

		while(Iter != m_pUIListHeader ) {
			pbInterfaceUI* pUI = Iter->getKernel();
			pUI->SetV2ParentPos(m_vParentPos[iX]+m_vPos[iX], m_vParentPos[iY]+m_vPos[iY]);
			Iter = Iter->getNext();
		}
	}
}

////----------------------------------------------------- ��ġ �Ұ�  UI------------------------------------------------------------------------------///////
pbUI_Untouchable::pbUI_Untouchable(){
}
pbUI_Untouchable::~pbUI_Untouchable() {
};

////----------------------------------------------------- ��ġ ����  UI------------------------------------------------------------------------------///////
pbUI_Touchable::pbUI_Touchable() {
	m_pTouchArea = new pbTouchArea;
}
pbUI_Touchable::~pbUI_Touchable() {
	delete m_pTouchArea;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ Base UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbBaseUI::pbBaseUI()
{
}

pbBaseUI::~pbBaseUI()
{

}

void pbBaseUI::Update(float fTime)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ TouchBase UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbTouchBaseUI::pbTouchBaseUI(){
	m_fpTouchAction = NULL;
}

pbTouchBaseUI::~pbTouchBaseUI(){
}

void pbTouchBaseUI::Update(float fTime){

}
void pbTouchBaseUI::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus) {
	switch (Touchstatus) {
//	case TOUCHSTATUS::TAPMOVE:
	case TOUCHSTATUS::TAPDOWN: {
		//x,y�� ���ؼ� �����Ǵ��� �Ѵ�.
		if (GetTouchArea()->InRect(x,y)) {
			LOGE("pbTouchBaseUI TAPDOWN");
			if( m_fpTouchAction != NULL )
				(*m_fpTouchAction)();
		}
		break;
	}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ����ġ UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbOneTouchUI::pbOneTouchUI(){
	m_bTouched = false;
}

pbOneTouchUI::~pbOneTouchUI(){

}

void pbOneTouchUI::Update(float fTime){
	if( m_bTouched ) {
		pbRenderProcess::RemoveRenderUI(this);
		pbUIProcessor::GetInstance()->DeleteUI(this);
		return;
	}

}
void pbOneTouchUI::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus) {
	switch (Touchstatus) {

//		case TOUCHSTATUS::TAPMOVE:
		case TOUCHSTATUS::TAPDOWN: {
			//x,y�� ���ؼ� �����Ǵ��� �Ѵ�.
			if (GetTouchArea()->InRect(x, y) ) {
				m_bTouched = true;
				return ;
			}
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- ������ UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbGaugeUI::pbGaugeUI() {

}

pbGaugeUI::~pbGaugeUI() {

}

void pbGaugeUI::Update(float fTime) {

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- �ֹ��� UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*pbAbilityPower_Indicator::pbAbilityPower_Indicator()
{
}

pbAbilityPower_Indicator::~pbAbilityPower_Indicator()
{


}
void pbAbilityPower_Indicator::Init()
{
	m_fMaxAbilityPoint = (float)pbUserData::SingleObject->GetMaxAbilityPoint();

	m_bNoHaveGauge = false;

	 m_vPos[0] = 0.0f;
	 m_vPos[1] = 0.0f;

	m_fGaugePercent = 1.0f;
	m_fGaugePosX = 0.0f;
	m_fDecreasePercent = 1.0f;
	m_fDecreasePosX = 0.0f;

	m_fAniTime = 0.0f;
	m_fMinusPercent = 0.0f;

}

void pbAbilityPower_Indicator::SetPos(float PosX, float PosY)
{
	m_vPos[0] = PosX;
	m_vPos[1] = PosY;

	m_fGaugePosX = m_fGaugeHalfWidth*(1.0f - m_fGaugePercent );
	m_fDecreasePosX = m_fGaugeHalfWidth*(1.0f - m_fDecreasePercent );//m_fGaugePosX + m_fGaugeHalfWidth*m_fGaugePercent + m_fGaugeHalfWidth*m_fDecreasePercent;


}

void pbAbilityPower_Indicator::SetVertexIndex(GLuint BodyIndex, GLuint TextIndex)
{
	m_BodyVertexIndex =BodyIndex;
	m_TextVertexIndex= TextIndex;

	m_fGaugeHalfWidth =  pbDataStorage::GetVertexWidth(BodyIndex)/2;
	m_fGaugeHalfHeight =  pbDataStorage::GetVertexHeight(BodyIndex)/2;
	m_fTextHalfWidth =   pbDataStorage::GetVertexWidth(TextIndex)/2;
	m_fTextHalfHeight =   pbDataStorage::GetVertexHeight(TextIndex)/2;

	//SetPos(0,0);
}

void pbAbilityPower_Indicator::SetUVIndex(GLuint BodyIndex, GLuint TextIndex) {
	m_BodyUVIndex =BodyIndex; m_TextUVIndex = TextIndex;

	memcpy(m_GaugeUV, pbDataStorage::GetUVCoordPointer(BodyIndex), sizeof(GLfloat)*8);

	m_GaugeUV_WidthPercent = m_GaugeUV[4] - m_GaugeUV[0];
}

void pbAbilityPower_Indicator::Draw()
{
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	///------------Power Text--------------------
		//���̺?�� ���̵�� ã�´�
		pbDataStorage::BindVertexAndTexture(m_TextVertexIndex, m_TextUVIndex);

		//�ؽ�ó ���ε�
		glPushMatrix();
		glTranslatef( -m_fGaugeHalfWidth + m_fTextHalfWidth,  m_fTextHalfHeight,  0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

	///------------������--------------------
		pbDataStorage::BindVertex(m_BodyVertexIndex);
		pbDataStorage::BindTexture(m_BodyUVIndex);

		glTexCoordPointer(2,GL_FLOAT, 0,  m_GaugeUV);	//�ؽ�ó��ǥ(UV) �迭 �Ѱ��ֱ�

		///------------�پ��� ������--------------------
			glPushMatrix();
			glColor4f(1.0f, 1.0f, 1.0f, 0.4f);

			m_GaugeUV[4] = m_GaugeUV[0] + m_GaugeUV_WidthPercent*m_fDecreasePercent;
			m_GaugeUV[6] = m_GaugeUV[2] + m_GaugeUV_WidthPercent*m_fDecreasePercent;

			glTranslatef( m_fDecreasePosX, -m_fGaugeHalfHeight ,  0);
			glScalef(m_fDecreasePercent, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


			glPopMatrix();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		///------------���� ������--------------------
		m_GaugeUV[4] = m_GaugeUV[0] + m_GaugeUV_WidthPercent*m_fGaugePercent;
		m_GaugeUV[6] = m_GaugeUV[2] + m_GaugeUV_WidthPercent*m_fGaugePercent;

		glPushMatrix();
		glTranslatef( m_fGaugePosX,  -m_fGaugeHalfHeight ,  0);
		glScalef(m_fGaugePercent, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();


	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

void pbAbilityPower_Indicator::Update(float fTime)
{
	///
	if(!m_bNoHaveGauge)
	{
		float UseAP = (float)pbUserData::SingleObject->GetUsedAbilityPoint();
		if( UseAP != 0.0f)
		{
			m_fMinusPercent = UseAP/m_fMaxAbilityPoint;
			pbUserData::SingleObject->ResetUsedAbilityPoint();

			if( m_fMinusPercent > 0.0f) {
				m_fGaugePercent -= m_fMinusPercent;
				m_fGaugePosX = - m_fGaugeHalfWidth*(1.0f - m_fGaugePercent );
			}
			else if( m_fMinusPercent < 0.0f) {
				m_fDecreasePercent = m_fGaugePercent - m_fMinusPercent;
				m_fDecreasePosX = - m_fGaugeHalfWidth*(1.0f - m_fDecreasePercent);
			}


			if( m_fGaugePercent < 0.0f)
			{
				m_bNoHaveGauge = true;
				m_fGaugePercent = 0.0f;
			}

		}

	}
	if( m_fMinusPercent > 0.0f) {
		if( m_fDecreasePercent > m_fGaugePercent)
		{
			m_fDecreasePercent = Lerp(m_fDecreasePercent, m_fGaugePercent, 0.05f);
			m_fDecreasePosX = - m_fGaugeHalfWidth*(1.0f - m_fDecreasePercent);
		}
	}
	else if( m_fMinusPercent < 0.0f) {
		m_fAniTime += fTime;

		m_fGaugePercent = Lerp(m_fDecreasePercent + m_fMinusPercent, m_fDecreasePercent, m_fAniTime);
		m_fGaugePosX = - m_fGaugeHalfWidth*(1.0f - m_fGaugePercent );

		if( m_fAniTime > 1.0f) {
			m_fAniTime = 0.0f;
			m_fGaugePercent = m_fDecreasePercent;

			m_fMinusPercent = 0.0f;
		}

	}

}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- ���ھ� ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*pbScore_Indicator::pbScore_Indicator()
{
}

pbScore_Indicator::~pbScore_Indicator()
{

}
void pbScore_Indicator::Init()
{
	 DataReset();
}

void pbScore_Indicator::SetVertexIndex(GLuint BodyIndex, GLuint TextIndex)
{
	m_BodyVertexIndex =BodyIndex;
	m_TextVertexIndex = TextIndex;

	float width = pbDataStorage::GetVertexWidth(BodyIndex);

	m_PlacementWidth = width;

	m_fTextHalfWidth =   pbDataStorage::GetVertexWidth(TextIndex)/2;
//	m_fTextHalfHeight =   pbDataStorage::GetVertexHeight(TextIndex)/2;
}

void pbScore_Indicator::DataReset()
{
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;

	m_NumberData = 0;
}

void pbScore_Indicator::SetUVIndex(GLuint StartNumberUVIndex, GLuint TextIndex)
{
	for(int i = 0; i < NUMBERING; i++)
	{
		m_NumberUVIndex[i] = StartNumberUVIndex + i;
	}

	m_TextUVIndex = TextIndex;
}

void pbScore_Indicator::Draw()
{
	///------------Power Text--------------------
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
		//�ؽ�ó ���ε�
		glPushMatrix();
		pbDataStorage::BindVertexAndTexture(m_TextVertexIndex, m_TextUVIndex);
		glTranslatef( -m_fTextHalfWidth - 10,  0.0f,  0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

		///--------------------------------------
		//���̺?�� ���̵�� ã�´�
		for(int i = 0; i < MAX_DIGITS; i++)		{
			glPushMatrix();
			pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_NumberUVIndex[m_DigitsNumber[i]]);
			//�ؽ�ó ���ε�
			glTranslatef(8 + ((float)i)*m_PlacementWidth, 0,  0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();
		}
	glPopMatrix();
}

void pbScore_Indicator::Update(float fTime)
{
	if( pbGlobalInGameVariable::PlusScore != 0)
	{
		m_NumberData += pbGlobalInGameVariable::PlusScore;
		 pbGlobalInGameVariable::PlusScore = 0;

		int DigitsNumber = m_NumberData;		//����ĭ�� ��ġ��  Score : [0] [1] [2] ... �� ���̹Ƿ� �ùٸ� ǥ���� ���� �������� �ִ´�
		for( int i = MAX_DIGITS - 1; i > 0; --i)
		{
			m_DigitsNumber[i] = DigitsNumber%10;
			DigitsNumber /= 10;
		}

		LOGfloatString("Real", m_NumberData);
		LOGfloatString("1", m_DigitsNumber[0]);
		LOGfloatString("10", m_DigitsNumber[1]);
		LOGfloatString("100", m_DigitsNumber[2]);
		LOGfloatString("1000", m_DigitsNumber[3]);
		LOGfloatString("10000", m_DigitsNumber[4]);
		LOGfloatString("100000", m_DigitsNumber[5]);
	}
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- �ε��ۼ�Ʈ ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*pbLoadingPercent_UI::pbLoadingPercent_UI()
{
}

pbLoadingPercent_UI::~pbLoadingPercent_UI()
{

}
void pbLoadingPercent_UI::Init()
{
	 DataReset();
}

void pbLoadingPercent_UI::SetVertexIndex(GLuint TextIndex, GLuint NumberIndex)
{
	m_BodyVertexIndex =NumberIndex;
	m_TextVertexIndex = TextIndex;

	float width = pbDataStorage::GetVertexWidth(NumberIndex);

	m_PlacementWidth = width;

	m_fTextHalfWidth =   pbDataStorage::GetVertexWidth(TextIndex)/2;
//	m_fTextHalfHeight =   pbDataStorage::GetVertexHeight(TextIndex)/2;
}

void pbLoadingPercent_UI::DataReset()
{
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;

	m_CurrentDigits = 1;
}

void pbLoadingPercent_UI::SetUVIndex(GLuint TextIndex ,GLuint StartNumberIndex)
{
	for(int i = 0; i < NUMBERING; i++)
	{
		m_NumberUVIndex[i] = StartNumberIndex + i;
	}

	m_TextUVIndex = TextIndex;
}

void pbLoadingPercent_UI::Draw()
{
	///------------Text--------------------
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
		//�ؽ�ó ���ε�
		glPushMatrix();
		pbDataStorage::BindVertexAndTexture(m_TextVertexIndex, m_TextUVIndex);
		glTranslatef( -m_fTextHalfWidth - 10,  0.0f,  0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

		///--------------------------------------
		glTranslatef( -m_PlacementWidth, 0,  0);

		glPushMatrix();
		int count = m_CurrentDigits - 1;
		for(int i = 0 ; i < m_CurrentDigits; ++i)		{
			pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_NumberUVIndex[m_DigitsNumber[count]]);
			glTranslatef( m_PlacementWidth, 0,  0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			count--;
		}
		glPopMatrix();

	glPopMatrix();
}

void pbLoadingPercent_UI::Update(float fTime)
{
}

void pbLoadingPercent_UI::SetLoadingPercentage(float fPercentage){
	int count = 0;
	int DigitsNumber =  (int)	fPercentage;

	while(1)
	{
		if( count < MAX_DIGITS) {
			m_DigitsNumber[count++] = DigitsNumber%10;
			DigitsNumber /= 10;

			if( DigitsNumber == 0 )
				break;
		}
		else
			break;
	}

	m_CurrentDigits = count;
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------���μ��� ------------------------------------------------------------------------------///////
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
		SingleObject->m_ControledUIStore = new pbLinkNode<pbInterfaceUI>;
		SingleObject->m_ControledUIStore->setHeader();

		LOGI("UIProcessor Create Complete");
	}


	LOGI("pbUIProcesser Initialized");
}

bool pbUIProcessor::LoadData(const char* filename) {
	//------------XML �Ľ�--------//
		char* buffer = pbProjectBeanFrame::npGetAssetBuffer(filename);
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
				double PosX, PosY, Width, Height;

				UIData->ToElement()->Attribute("ID", &ID);
				UIData->ToElement()->Attribute("X", &PosX);
				UIData->ToElement()->Attribute("Y", &PosY);
				UIData->ToElement()->Attribute("Width", &Width);
				UIData->ToElement()->Attribute("Height", &Height);

				SingleObject->CreateUIByType(Type, PosX, PosY, Width, Height);

				UIData = UIData->NextSibling();
			}

		LOGI("Done UI Xml parsing");

		xmlDoc.Clear();
		pbProjectBeanFrame::npCloseAssetBuffer(buffer);

		return true;
}

void pbUIProcessor::CreateUIByType(const char* Type, float X, float Y, float Width, float Height) {
	if( strcmp(Type, "BackPanel") == 0 ) {
		AddBackPanelUI(X, Y, Width, Height, 29);
	}
	else if( strcmp(Type, "Menu") == 0 ) {
		AddMenuButtonUI(X, Y,Width, Height, 43);
	}
	else if( strcmp(Type, "Help") == 0 ) {
		AddHelpButtonUI(X, Y, Width, Height,  44);
	}
/*	else if( strcmp(Type, "APGauge") == 0 ) {
		AddAbillityPointUI(X, Y, 11, 10, 31, 30);
	}
	else if( strcmp(Type, "Score") == 0 ) {
		AddScoreUI(X, Y, 13, 12, 33, 32);
	}*/
}

pbInterfaceUI* pbUIProcessor::AddTopSideInfoUI(float X, float Y, pbInterfaceUI* Menu, pbInterfaceUI* Help, pbInterfaceUI* Score, pbInterfaceUI* Gauge) {
	pbBaseUI* newUI = new pbBaseUI();

	newUI->GetBaseDrawUnit()->SetID(0);
	newUI->GetBaseDrawUnit()->SetV2Pos(X, Y);

	newUI->AddChildUnit(Menu);
	newUI->AddChildUnit(Help);
//	newUI->AddChildUnit(Score);
//	newUI->AddChildUnit(Gauge);

	//TODO::  UV�� �ް� ���������� ������ ���� �����. (��� �ڽĵ��� ��ġ�� ũ��� �����Ȱ�)

	return newUI;
}

//----------------------------BackPanel-------------------------------------//
pbInterfaceUI* pbUIProcessor::AddBackPanelUI(float X, float Y, float Width, float Height, GLuint UVIndex) {
	pbBaseUI* newUI = new pbBaseUI();

	newUI->SetV2Pos(X, Y);

	newUI->GetBaseDrawUnit()->SetID(0);
	newUI->GetBaseDrawUnit()->SetV2Pos(0, 0);
	newUI->GetBaseDrawUnit()->SetUVIndex(UVIndex);
	newUI->GetBaseDrawUnit()->SetSizeWH(Width, Height);

	pbTouchBaseUI* childUI = new pbTouchBaseUI();
	childUI->SetV2Pos(0, 0);

	childUI->GetBaseDrawUnit()->SetID(0);
	childUI->GetBaseDrawUnit()->SetV2Pos(0, 0);
	childUI->GetBaseDrawUnit()->SetUVIndex(43);
	childUI->GetBaseDrawUnit()->SetSizeWH(80, 80);

	newUI->AddChildUnit(childUI);


	pbBaseUI* childUI_2 = new pbBaseUI();
	childUI_2->SetV2Pos(100, 100);

	childUI_2->GetBaseDrawUnit()->SetID(0);
	childUI_2->GetBaseDrawUnit()->SetV2Pos(0, 0);
	childUI_2->GetBaseDrawUnit()->SetUVIndex(5);
	childUI_2->GetBaseDrawUnit()->SetSizeWH(80, 80);


	childUI->AddChildUnit(childUI_2);

	pbBaseUI* childUI_3 = new pbBaseUI();
	childUI_3->SetV2Pos(100, -200);


	childUI_3->GetBaseDrawUnit()->SetID(0);
	childUI_3->GetBaseDrawUnit()->SetV2Pos(0, 0);
	childUI_3->GetBaseDrawUnit()->SetUVIndex(20);
	childUI_3->GetBaseDrawUnit()->SetSizeWH(80, 80);


	childUI_2->AddChildUnit(childUI_3);

	pbTouchBaseUI* childUI_4 = new pbTouchBaseUI();
	childUI_4->SetV2Pos(0, 100);

	childUI_4->GetBaseDrawUnit()->SetID(0);
	childUI_4->GetBaseDrawUnit()->SetV2Pos(0, 0);
	childUI_4->GetBaseDrawUnit()->SetUVIndex(42);
	childUI_4->GetBaseDrawUnit()->SetSizeWH(80, 80);

	childUI_2->AddChildUnit(childUI_4);

/*	pbDrawUnit* pDrawUnit_Child = new pbDrawUnit;
	pDrawUnit_Child->SetID(1);
	pDrawUnit_Child->SetV2Pos(0, 0);
	pDrawUnit_Child->SetUVIndex(43);
	pDrawUnit_Child->SetSizeWH(40, 38);

	newUI->GetBaseDrawUnit()->AddChildUnit(pDrawUnit_Child);

	pbDrawUnit* pDrawUnit_Child_1 = new pbDrawUnit;
	pDrawUnit_Child_1->SetID(2);
	pDrawUnit_Child_1->SetV2Pos(50, 0);
	pDrawUnit_Child_1->SetUVIndex(42);
	pDrawUnit_Child_1->SetSizeWH(40, 38);

	newUI->GetBaseDrawUnit()->AddChildUnit(pDrawUnit_Child_1);

	pDrawUnit_Child = new pbDrawUnit;
	pDrawUnit_Child->SetID(2);
	pDrawUnit_Child->SetV2Pos(0, 50);
	pDrawUnit_Child->SetUVIndex(11);
	pDrawUnit_Child->SetSizeWH(40, 38);

	pDrawUnit_Child_1->AddChildUnit(pDrawUnit_Child);

	pDrawUnit_Child = new pbDrawUnit;
	pDrawUnit_Child->SetID(2);
	pDrawUnit_Child->SetV2Pos(0, -50);
	pDrawUnit_Child->SetUVIndex(32);
	pDrawUnit_Child->SetSizeWH(40, 38);

	pDrawUnit_Child_1->AddChildUnit(pDrawUnit_Child);*/

	registControled(newUI );
	pbRenderProcess::RegistRenderUI(newUI );

	return newUI;
}

//----------------------------Menu-------------------------------------//
pbInterfaceUI* pbUIProcessor::AddMenuButtonUI(float X, float Y, float Width, float Height, GLuint UVIndex) {
	pbTouchBaseUI* newUI = new pbTouchBaseUI();
	newUI->SetV2Pos(X, Y);

	newUI->GetBaseDrawUnit()->SetID(0);
	newUI->GetBaseDrawUnit()->SetV2Pos(0, 0);
	newUI->GetBaseDrawUnit()->SetUVIndex(UVIndex);
	newUI->GetBaseDrawUnit()->SetSizeWH(Width, Height);
	newUI->GetTouchArea()->setTouchArea(Width, Height);
	newUI->GetTouchArea()->movePositon(X, Y);

	registControled(newUI );
	pbRenderProcess::RegistRenderUI(newUI );
	pbTouchLayer::registerObserver(newUI);

	return newUI;
}

//----------------------------Help-------------------------------------//
pbInterfaceUI* pbUIProcessor::AddHelpButtonUI(float X, float Y, float Width, float Height, GLuint UVIndex) {
	pbTouchBaseUI* newUI = new pbTouchBaseUI();
	newUI->SetV2Pos(X, Y);

	newUI->GetBaseDrawUnit()->SetID(0);
	newUI->GetBaseDrawUnit()->SetV2Pos(0, 0);
	newUI->GetBaseDrawUnit()->SetUVIndex(UVIndex);
	newUI->GetBaseDrawUnit()->SetSizeWH(Width, Height);
	newUI->GetTouchArea()->setTouchArea(Width, Height);
	newUI->GetTouchArea()->movePositon(X, Y);

	registControled(newUI );
	pbRenderProcess::RegistRenderUI(newUI );
	pbTouchLayer::registerObserver(newUI);

	return newUI;
}

//----------------------------AddOneTouchUI-------------------------------------//
pbInterfaceUI* pbUIProcessor::AddOneTouchUI(float X, float Y, float Width, float Height, GLuint UVIndex) {
	pbOneTouchUI* newUI = new pbOneTouchUI();
	newUI->SetV2Pos(X, Y);

	newUI->GetBaseDrawUnit()->SetID(0);
	newUI->GetBaseDrawUnit()->SetV2Pos(0, 0);
	newUI->GetBaseDrawUnit()->SetUVIndex(UVIndex);
	newUI->GetBaseDrawUnit()->SetSizeWH(Width, Height);
	newUI->GetTouchArea()->setTouchArea(Width, Height);
	newUI->GetTouchArea()->movePositon(X, Y);

	pbTouchLayer::registerObserver(newUI);
	registControled(newUI );
	pbRenderProcess::RegistRenderUI(newUI );
	return newUI;
}
/*
//----------------------------Score-------------------------------------//
pbUI* pbUIProcessor::AddScoreUI(float X, float Y, GLuint NumberVertexIndex, GLuint TextVertexIndex, GLuint NumberUVIndex, GLuint TextUVIndex) {
	pbUI* newUI = new pbScore_Indicator();
	((pbScore_Indicator*)newUI) ->Init();
	((pbScore_Indicator*)newUI) ->SetVertexIndex(NumberVertexIndex, TextVertexIndex);
	((pbScore_Indicator*)newUI) ->SetUVIndex(NumberUVIndex, TextUVIndex);

	//newUI ->SetID(ID);   // ID������ ����� �غ��� ��
	newUI ->SetPos( X, Y);
	registControled(newUI );
	pbRenderProcess::RegistRenderUI(newUI );
	return newUI;

}
//----------------------------APGauge-------------------------------------//
pbUI* pbUIProcessor::AddAbillityPointUI(float X, float Y, GLuint GaugeVertexIndex, GLuint TextVertexIndex, GLuint GaugeUVIndex, GLuint TextUVIndex) {
	pbUI* newUI = new pbAbilityPower_Indicator();
	((pbAbilityPower_Indicator*)newUI) ->Init();
	((pbAbilityPower_Indicator*)newUI) ->SetVertexIndex(GaugeVertexIndex, TextVertexIndex);
	((pbAbilityPower_Indicator*)newUI) ->SetUVIndex(GaugeUVIndex, TextUVIndex);

	//newUI ->SetID(ID);   // ID������ ����� �غ��� ��
	newUI ->SetPos( X, Y);
	registControled(newUI );
	pbRenderProcess::RegistRenderUI(newUI );
	return newUI;

}

pbUI* pbUIProcessor::AddLoadingPercentUI(float X, float Y, GLuint TextVertexIndex, GLuint NumberVertexIndex, GLuint TextUVIndex, GLuint NumberUVIndex) {
	pbUI* newUI = new pbLoadingPercent_UI();
	((pbLoadingPercent_UI*)newUI) ->Init();
	((pbLoadingPercent_UI*)newUI) ->SetVertexIndex(TextVertexIndex, NumberVertexIndex);
	((pbLoadingPercent_UI*)newUI) ->SetUVIndex(TextUVIndex, NumberUVIndex);

	//newUI ->SetID(ID);   // ID������ ����� �غ��� ��
	newUI ->SetPos( X, Y);
	registControled(newUI );
	pbRenderProcess::RegistRenderUI(newUI );
	return newUI;
}
*/


void pbUIProcessor::Update(float time){
	pbLinkNode<pbInterfaceUI>* iterator;
	pbLinkNode<pbInterfaceUI>* head = m_ControledUIStore;

	iterator = head->getNext();
	while( iterator != head ) {
		pbInterfaceUI* pUI = iterator->getKernel();
		iterator = iterator->getNext();

		pUI->Update(time);
	}

}

void pbUIProcessor::ClearDataStore() {
	pbLinkNode<pbInterfaceUI>::ClearListAndDeleteKernel(m_ControledUIStore);

	LOGfloatString("ClearUI :", m_UICounts);

	m_UICounts = 0;
	LOGI("pbUIProcesser ClearDataStore");
}

void pbUIProcessor::Release(){
	if( SingleObject != NULL) {
		pbLinkNode<pbInterfaceUI>::destroyListAndDeleteKernel(SingleObject->m_ControledUIStore);

		delete SingleObject;
		LOGI("pbUIProcesser Release");

		SingleObject = NULL;
	}
}

void pbUIProcessor::registControled(pbInterfaceUI* pUI){
	pbLinkNode<pbInterfaceUI>* pTargetNode = pbLinkNode<pbInterfaceUI>::makeLinkNode(pUI);
	pbLinkNode<pbInterfaceUI>::addTail(pTargetNode,m_ControledUIStore);

	m_UICounts++;
}

void pbUIProcessor::removeControled(pbInterfaceUI* pUI){
	pbLinkNode<pbInterfaceUI>* pTargetNode = pbLinkNode<pbInterfaceUI>::makeLinkNode(pUI);
	pbLinkNode<pbInterfaceUI>::findDelete(pTargetNode,m_ControledUIStore);
}

void pbUIProcessor::DeleteUI(pbInterfaceUI* pUI) {
	if( pUI != NULL ) {
		removeControled(pUI);
		delete pUI;

		m_UICounts--;
	}
}
