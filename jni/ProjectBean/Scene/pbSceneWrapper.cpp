/*
 * pbSceneManager.cpp
 *
 *  Created on: 2012. 12. 7.
 *      Author: NitroSoft
 */

#include "pbSceneWrapper.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbSceneWrapper  Base Class------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLfloat pbSceneWrapper::m_FadeVertex[12] = {0,};

pbSceneWrapper::pbSceneWrapper() {
	m_RenderListHeader =new RenderList;
	m_RenderListHeader->setHeader();

	m_StageTrigger = new pbStageTrigger();

	SetVertexByCenter(m_FadeVertex, 800, 480);
	m_FadeValue = 0.0f;
	m_FadeControl = 0.0f;
	m_FadeOn = false;
}

pbSceneWrapper::~pbSceneWrapper() {
	RenderList::destroyList(m_RenderListHeader);
	delete m_StageTrigger;
}

void pbSceneWrapper::DrawScene() {
	RenderList* iterator;
	RenderList* head = m_RenderListHeader;
	iterator = head->getNext();
	while (iterator != head) {
		npDrawable* pkernel = iterator->getKernel();
		iterator = iterator->getNext();
		npRenderprocess::getInstance().DoDraw((*pkernel));
	}

	if(m_FadeOn ) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPushMatrix();
		glTranslatef(400, 240, 0);
		glColor4f(0.0f, 0.0f, 0.0f, m_FadeValue);
		glVertexPointer(3, GL_FLOAT, 0, m_FadeVertex);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glPopMatrix();
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
//	LOGE("DEBUG pbSceneWrapper : DrawScene complete");
}

void pbSceneWrapper::RegistToRenderList(npDrawable* pDrawable) {
	if(pDrawable != NULL ) {
		RenderList::addTail(pDrawable, 	m_RenderListHeader);
		LOGI("pbSceneWrapper::RegistRenderList");
	}
	else
		LOGE("pbSceneWrapper::RegistRenderList Drawble is NULL");
}

void pbSceneWrapper::RemoveToRenderList(npDrawable* pDrawable) {
	if(pDrawable != NULL ) {
		RenderList::findDelete(pDrawable, 	m_RenderListHeader);
		LOGI("pbSceneWrapper::RemoveToRenderList");
	}
	else
		LOGE("pbSceneWrapper::RemoveToRenderList Drawble is NULL");
}

void pbSceneWrapper::ResetFadeValues() {
	m_FadeValue = 1.0f;
	m_FadeControl = 0.0f;
}

void pbSceneWrapper::StartFadeIn(float fFadeTime) {
	m_FadeOn = true;
	m_FadeValue = 1.0f;
	m_FadeControl = -(1/fFadeTime);

	LOGI(" pbSceneWrapper::StartFadeIn");
}
void pbSceneWrapper::StartFadeOut(float fFadeTime) {
	m_FadeOn = true;
	m_FadeValue = 0.0f;
	m_FadeControl = 1/fFadeTime;
	LOGI(" pbSceneWrapper::StartFadeOut");
}

void pbSceneWrapper::FadeUpdate(float fTime) {
	if( m_FadeControl != 0.0f) {
		m_FadeValue += m_FadeControl*fTime;

		if( m_FadeValue < 0.0f ) {
			m_FadeValue =  0.0f;
			m_FadeControl =  0.0f;
			m_FadeOn = false;
		}
		else 	if( m_FadeValue > 1.0f ) {
			m_FadeValue = 1.0;
			m_FadeControl =  0.0f;
		}
	}

}

void pbSceneWrapper::ClearToRenderList() {
	RenderList::clearList(m_RenderListHeader);
	LOGI("pbSceneWrapper::ClearToRenderList");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbIntroSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbIntroSceneWrapper::pbIntroSceneWrapper(const char* SceneTag) {
	m_IntroBG = NULL;
	SetTag(SceneTag);	//이 씬의 기본 태그를 지정
}

pbIntroSceneWrapper::~pbIntroSceneWrapper() {

}

void pbIntroSceneWrapper::InitializeScene() {
	StartFadeIn(0.5f);
	TAGDATA BaseTagData;
	BaseTagData.SetData("run", 800, 480);
	m_IntroBG = pbBackgroundProcessor::GetInstance().AddTouchableBackGround(400, 240, BaseTagData);
	RegistToRenderList(m_IntroBG);

	LOGI("pbIntroSceneWrapper InitializeScene Complete");
}

void pbIntroSceneWrapper::UpdateScene(float fTime) {
	FadeUpdate(fTime);
	pbBackgroundProcessor::GetInstance().Update(fTime);

	if( m_IntroBG != NULL ) {
		if( m_IntroBG->IsTouched() ) {
			StartFadeOut(0.5f);
			m_IntroBG->ResetTouched();
		 }
	}

	if( IsFadeOutEnd()) {
		pbSceneNavigator::GetInstance().SearchAndReadyToMoveScene(SCENESTATE::ACTION_FORWARD);
		ResetFadeValues();
	}
}

void pbIntroSceneWrapper::ClearScene() {
	ClearToRenderList();
	TouchLayer::GetInstance().ClearRegistedList();
	pbBackgroundProcessor::GetInstance().ClearDataStore();
	m_IntroBG = NULL;
	LOGI("pbIntroSceneWrapper ClearScene Complete");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbPlaySceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbPlaySceneWrapper::pbPlaySceneWrapper(const char* SceneTag) {
	SetTag(SceneTag);	//이 씬의 기본 태그를 지정
}

pbPlaySceneWrapper::~pbPlaySceneWrapper() {
}

void pbPlaySceneWrapper::InitializeScene() {
	StartFadeIn(0.5f);
	pbEffectManager::GetInstance()->SetSceneTag(GetTag());
	GetStageTrigger()->Initialize();

	pbInventory::GetInstance().DataTransferToStageValue();

	TAGDATA BaseTagData, SecondTagData;
	//background
	BaseTagData.SetData("ci", 800, 480);
	pbBackground* pCreateBG = pbBackgroundProcessor::GetInstance().AddScrollBackGround(400, 240, BaseTagData, 0.1f);
	RegistToRenderList(pCreateBG);

	BaseTagData.SetData("ci", 400, 240);
	pCreateBG = pbBackgroundProcessor::GetInstance().AddMoveBackGround(0, 220, BaseTagData, 0.5f);
	RegistToRenderList(pCreateBG);

	BaseTagData.SetData("ci", 800, 100);
	pCreateBG = pbBackgroundProcessor::GetInstance().AddScrollBackGround(400, 50, BaseTagData, 1.0f);
	RegistToRenderList(pCreateBG);


	//----------------UI------------------------------//
	BaseTagData.SetData("ci", 800, 50);
	pbUI* createUI = pbUIProcessor::GetInstance()->AddBackPanelUI(400, 450, BaseTagData);
	this->RegistToRenderList(createUI);
	//라이프 UI 추가
	BaseTagData.SetData("ci", 90, 30);
	SecondTagData.SetData("run", 25, 35);
	pbNumber_Indicator* LifeUI = pbUIProcessor::GetInstance()->AddNumberUI(100, 450, BaseTagData, SecondTagData, 1, &(pbStageValue::CalcLifeData));
	LifeUI->SetNumber(pbStageValue::GetLifeTotal());
	this->RegistToRenderList(LifeUI);
	//스코어 UI 추가
	BaseTagData.SetData("ci", 100, 30);
	SecondTagData.SetData("run", 25, 35);
	pbNumber_Indicator* ScoreUI = pbUIProcessor::GetInstance()->AddNumberUI(125 + 110, 450, BaseTagData, SecondTagData, 9, &(pbStageValue::CalcScoreData));
	ScoreUI->SetNumber( pbStageValue::GetScoreTotal());
	this->RegistToRenderList(ScoreUI);
	// 메뉴버튼
	BaseTagData.SetData("ci", 70, 40);
	createUI = pbUIProcessor::GetInstance()->AddButtonUI(760, 450, BaseTagData, &(pbPlaySceneWrapper::MenuTouch));
	this->RegistToRenderList(createUI);
	// 맵 네비게이트
	pbStageValue::SetStageMaxLength(40000);
	BaseTagData.SetData("ci", 250, 25);
	createUI = pbUIProcessor::GetInstance()->AddGaugeUI(590, 450, "run", BaseTagData, pbStageValue::GetStageMaxLength(), &(pbStageValue::GetStageX));
	this->RegistToRenderList(createUI);

	pbComboManager::GetInstance()->LoadData();
	RegistToRenderList(pbComboManager::GetInstance());

	// 캐릭터
	pbCharacter::GetInstance()->LoadData(GetTag());
	pbCharacter::GetInstance()->SetPos(-200, 240);
	pbCharacter::GetInstance()->SetConditionPos(72.f, 240.f);
	pbCharacter::GetInstance()->SetTouchFunction(&(pbCharacter::PlayGame_TouchFunc));
	RegistToRenderList(pbCharacter::GetInstance());
	GetStageTrigger()->AddPosState(0, &(pbCharacter::Appeared));
	GetStageTrigger()->AddIDState(pbCharacter::WALKOUT, &(pbCharacter::WalkOut));

	// Fever게이지
	BaseTagData.SetData("ci", 150, 20);
	createUI = pbUIProcessor::GetInstance()->AddGaugeUI_RelativePos(pbCharacter::GetMarionette()->GetV2Pos(), 0, -((pbCharacter::GetInstance()->GetHeight()/2) + 15), "run", BaseTagData, pbStageValue::MAX_FEVERGAUGE, &(pbStageValue::GetFeverGauge));
	this->RegistToRenderList(createUI);

	// 보스
	pbBoss::GetInstance()->LoadData();
	pbBoss::GetMarionette()->SetPosX(1200);pbBoss::GetMarionette()->SetPosY(240);
	pbBoss::GetInstance()->SetConditionPos(690.0f, 240.0f);
	RegistToRenderList(pbBoss::GetInstance());
	GetStageTrigger()->AddPosState(400, &(pbBoss::Approaching));
	GetStageTrigger()->AddIDState(pbBoss::DIE, &(pbBoss::PostDieProcess));

	LOGI("pbPlaySceneWrapper InitializeScene Complete");
}

void pbPlaySceneWrapper::UpdateScene(float fTime) {
	FadeUpdate(fTime);
	if(!GetStageTrigger()->IsPaused())
	{
		GetStageTrigger()->Update(fTime);

		pbBackgroundProcessor::GetInstance().Update(fTime);
		pbUIProcessor::GetInstance()->Update(fTime);
		pbCharacter::GetInstance()->Update(fTime);
		pbBoss::GetInstance()->Update(fTime);
		pbEffectManager::GetInstance()->Update(fTime);

	}
}

void pbPlaySceneWrapper::MenuTouch() {
	pbStageValue::IncreaseFeverGauge(100);
}

void pbPlaySceneWrapper::ClearScene() {
	ResetFadeValues();
	ClearToRenderList();
	TouchLayer::GetInstance().ClearRegistedList();
	pbBackgroundProcessor::GetInstance().ClearDataStore();
	pbUIProcessor::GetInstance()->ClearDataStore();
	pbCharacter::GetInstance()->ClearDataStore();
	pbBoss::GetInstance()->ClearDataStore();
	pbEffectManager::GetInstance()->ClearDataStore();
	pbComboManager::GetInstance()->ClearDataStore();

/*	pbNoteProcessor::GetInstance()->ClearDataStore();

	pbGuideLineGenerator::GetInstance()->ClearDataStore();
	*/
	LOGI("pbPlaySceneWrapper ClearScene Complete");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbScoreSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbResultSceneWrapper::pbResultSceneWrapper(const char* SceneTag) {
	SetTag(SceneTag);	//이 씬의 기본 태그를 지정
	m_ResultViewer = new pbResultViewer();
}

pbResultSceneWrapper::~pbResultSceneWrapper() {
	delete m_ResultViewer;
}

void pbResultSceneWrapper::InitializeScene() {
	StartFadeIn(0.3f);
	GetStageTrigger()->Initialize();
	RegistToRenderList(m_ResultViewer);

	TAGDATA BaseTagData, SecondTagData;
	BaseTagData.SetData("ci", 150, 50);
	SecondTagData.SetData("run", 50, 50);

	m_ResultViewer->SetPos(200, 0);
	m_ResultViewer->PushBackScoreView(0, 430, BaseTagData, SecondTagData, pbStageValue::GetScoreTotal());
	m_ResultViewer->PushBackScoreView(0, 330, BaseTagData, SecondTagData, pbStageValue::GetScoreTotal());
	m_ResultViewer->PushBackScoreView(0, 230, BaseTagData, SecondTagData, pbStageValue::GetScoreTotal());
	m_ResultViewer->PushBackScoreView(0, 130, BaseTagData, SecondTagData, pbStageValue::GetScoreTotal());

	// 캐릭터
	pbCharacter::GetInstance()->LoadData(GetTag());
	pbCharacter::GetInstance()->SetTouchFunction(&(pbCharacter::Result_TouchFunc));
	pbCharacter::GetInstance()->SetPos(600.0f, 100.0f);
	pbCharacter::GetInstance()->SetConditionPos(600.f, 100.f);
	pbCharacter::GetMarionette()->SelectMoveState(pbCharacter::WEAVING_UP);
	GetStageTrigger()->AddIDState(pbCharacter::WALKOUT, &(pbCharacter::WalkOut));
	RegistToRenderList(pbCharacter::GetInstance());

	LOGI("pbResultSceneWrapper InitializeScene Complete");
}

void pbResultSceneWrapper::UpdateScene(float fTime) {
	FadeUpdate(fTime);

	m_ResultViewer->Update(fTime);
	pbCharacter::GetInstance()->Update(fTime);

}

void pbResultSceneWrapper::ClearScene() {
	ResetFadeValues();
	ClearToRenderList();
	pbCharacter::GetInstance()->ClearDataStore();
	m_ResultViewer->ClearDataStore();
	TouchLayer::GetInstance().ClearRegistedList();
	//pbBackgroundProcessor::GetInstance().ClearDataStore();

	LOGI("pbResultSceneWrapper ClearScene Complete");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbShopSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbShopSceneWrapper::pbShopSceneWrapper(const char* SceneTag) {
	SetTag(SceneTag);	//이 씬의 기본 태그를 지정
}

pbShopSceneWrapper::~pbShopSceneWrapper() {
}

void pbShopSceneWrapper::InitializeScene() {
	StartFadeIn(0.3f);
	GetStageTrigger()->Initialize();
	//골드 UI 추가
	TAGDATA BaseTagData, SecondTagData;
	BaseTagData.SetData("ci", 100, 30);
	SecondTagData.SetData("run", 25, 35);
	pbNumber_Indicator* GoldUI = pbUIProcessor::GetInstance()->AddNumberUI(650, 450, BaseTagData, SecondTagData, 5, &(pbGoldPouch::CalcGold));
	GoldUI->SetNumber( pbGoldPouch::GetInstance().GetGold());
	this->RegistToRenderList(GoldUI);

	// 캐릭터
	pbCharacter::GetInstance()->LoadData(GetTag());
	pbCharacter::GetInstance()->SetTouchFunction(&(pbCharacter::Shop_TouchFunc) );
	pbCharacter::GetInstance()->SetPos(600.0f, 100.0f);
	pbCharacter::GetInstance()->SetConditionPos(600.f, 100.f);
	pbCharacter::GetMarionette()->SelectMoveState(pbCharacter::WEAVING_UP);
	GetStageTrigger()->AddIDState(pbCharacter::WALKOUT, &(pbCharacter::WalkOut));
	this->RegistToRenderList(pbCharacter::GetInstance());

	//아이템
	pbShop::GetInstance().LoadData();
	pbShop::GetInstance().SetPos(0,0);
	this->RegistToRenderList(&pbShop::GetInstance());
	LOGI("pbShopSceneWrapper InitializeScene Complete");

	pbStageValue::ResetShopRoute();
}

void pbShopSceneWrapper::UpdateScene(float fTime) {
	FadeUpdate(fTime);

	pbUIProcessor::GetInstance()->Update(fTime);
	pbCharacter::GetInstance()->Update(fTime);
	pbShop::GetInstance().Update(fTime);
}

void pbShopSceneWrapper::ClearScene() {
	ResetFadeValues();
	ClearToRenderList();

	pbUIProcessor::GetInstance()->ClearDataStore();
	pbCharacter::GetInstance()->ClearDataStore();
	pbShop::GetInstance().ClearData();

	LOGI("pbShopSceneWrapper ClearScene Complete");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbSceneManager------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbSceneManager::pbSceneManager() {
	m_pCurrentScene = NULL;

}

pbSceneManager::~pbSceneManager() {
	pbSceneMap::iterator Iter;
	for( Iter = m_SceneStore.begin(); Iter != m_SceneStore.end(); ++Iter) {
		pbSceneWrapper* pSecond = Iter->second;
		m_SceneStore.erase(Iter);
		delete pSecond;
	}
}

pbSceneManager& pbSceneManager::getInstance(){
	static pbSceneManager singletone;
	return singletone;
}

void pbSceneManager::AddScene(sceneTag Tag, pbSceneWrapper* pScene) {
	m_SceneStore.insert(std::pair<sceneTag, pbSceneWrapper*>(Tag, pScene));
}

void pbSceneManager::SelectScene(sceneTag SceneTag) {
	pbSceneMap::iterator Iter = m_SceneStore.find(SceneTag);
	if( Iter != m_SceneStore.end() ) {
		m_pCurrentScene = Iter->second;
		LOGE("[DEBUG]pbSceneManager::SelectScene success" );
	}
	else {
		m_pCurrentScene = NULL;
		LOGE("[DEBUG]pbSceneManager::SelectScene FAIL" );
	}

}

void pbSceneManager::Draw() {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->DrawScene();
}
void pbSceneManager::Update(float fTime) {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->UpdateScene(fTime);
}

void pbSceneManager::RegistRenderToCurrentScene(npDrawable* pDrawable) {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->RegistToRenderList(pDrawable);
}

void pbSceneManager::RegistRenderToScene(sceneTag Tag, npDrawable* pDrawable) {
	pbSceneMap::iterator Iter = m_SceneStore.find(Tag);
	if( Iter != m_SceneStore.end() ) {
		Iter->second->RegistToRenderList(pDrawable);
	}
}

void pbSceneManager::RemoveRenderToCurrentScene(npDrawable* pDrawable) {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->RemoveToRenderList(pDrawable);
}

void pbSceneManager::RemoveRenderToScene(sceneTag Tag, npDrawable* pDrawable) {
	pbSceneMap::iterator Iter = m_SceneStore.find(Tag);
	if( Iter != m_SceneStore.end() ) {
		Iter->second->RemoveToRenderList(pDrawable);
	}
}


