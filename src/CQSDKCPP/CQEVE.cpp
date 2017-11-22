/*
���ļ�������EX�¼���ʵ��
*/
#include "..\CQSDK\CQEVE_ALL.h"

#include "..\CQSDK\CQAPI_EX.h"
#include "..\CQSDK\CQTools.h"
#include "..\CQSDK\Unpack.h"

#include <windows.h>

using namespace CQ;

void CQ::EVE::message_ignore() { _EVEret = ��Ϣ_����; }
void CQ::EVE::message_block() { _EVEret = ��Ϣ_����; }

bool CQ::EVEMsg::isSystem() const { return fromQQ == 1000000; }

Font::Font(int Font) { RtlMoveMemory(static_cast<void*>(this), reinterpret_cast<void *>(Font), 20); }

CQ::EVEMsg::EVEMsg(int subType, int msgId, long long fromQQ, std::string message, int font)
	: subType(subType), msgId(msgId), fromQQ(fromQQ), message(message) , font(font){}

//��ʵ�û�
bool CQ::EVEMsg::isUser() const {
	switch (fromQQ)
	{
	case 1000000:// ϵͳ��ʾ
	case 80000000:// ����
		return false;
	default:
		return true;
	}
}

CQ::EVEGroupMsg::EVEGroupMsg(int subType, int msgId, long long fromGroup, long long fromQQ, const char * fromAnonymous, const char * msg, int font)
	:EVEMsg(subType, msgId, fromQQ, msg, font), fromGroup(fromGroup), fromAnonymousToken(fromAnonymous), fromAnonymousInfo() {}

CQ::EVEGroupMsg::~EVEGroupMsg() { if (fromAnonymousInfo != nullptr) delete fromAnonymousInfo; }


bool CQ::EVEGroupMsg::isAnonymous() const { return fromQQ == 80000000; }


AnonymousInfo & CQ::EVEGroupMsg::getFromAnonymousInfo() //throw(std::exception_ptr)
{
	if (isAnonymous())
		return
		fromAnonymousInfo != nullptr
		? *fromAnonymousInfo
		: *(fromAnonymousInfo = new AnonymousInfo(fromAnonymousToken));
	else
		throw std::exception_ptr();
}

bool CQ::EVEGroupMsg::setGroupKick(bool refusedAddAgain)
{

	return !CQ::setGroupKick(fromGroup, fromQQ, refusedAddAgain);
}

bool CQ::EVEGroupMsg::setGroupBan(long long banTime)
{
	if (isAnonymous()) {
		return !CQ::setGroupAnonymousBan(fromGroup, fromAnonymousToken, banTime);
	}
	else {
		return !CQ::setGroupBan(fromGroup, fromQQ, banTime);
	}
}

bool CQ::EVEGroupMsg::setGroupAdmin(bool isAdmin)
{
	return !CQ::setGroupAdmin(fromGroup, fromQQ, isAdmin);
}

bool CQ::EVEGroupMsg::setGroupSpecialTitle(std::string ר��ͷ��, long long ����ʱ��)
{
	return !CQ::setGroupSpecialTitle(fromGroup, fromQQ, ר��ͷ��, ����ʱ��);
}

bool CQ::EVEGroupMsg::setGroupWholeBan(bool ��������)
{
	return CQ::setGroupWholeBan(fromGroup, ��������);
}

bool CQ::EVEGroupMsg::setGroupAnonymous(bool ��������)
{
	return CQ::setGroupAnonymous(fromGroup, ��������);
}

bool CQ::EVEGroupMsg::setGroupCard(std::string ����Ƭ_�ǳ�)
{
	return CQ::setGroupCard(fromGroup, fromQQ, ����Ƭ_�ǳ�);
}

bool CQ::EVEGroupMsg::setGroupLeave(bool �Ƿ��ɢ)
{
	return CQ::setGroupLeave(fromGroup, �Ƿ��ɢ);
}

GroupMemberInfo CQ::EVEGroupMsg::getGroupMemberInfo(bool ��ʹ�û���)
{
	return CQ::getGroupMemberInfo(fromGroup, fromQQ, ��ʹ�û���);
}

std::vector<GroupMemberInfo> CQ::EVEGroupMsg::getGroupMemberList()
{
	return CQ::getGroupMemberList(fromGroup);
}

CQ::EVEPrivateMsg::EVEPrivateMsg(int subType, int msgId, long long fromQQ, const char * msg, int font)
	: EVEMsg(subType, msgId, fromQQ, msg, font) {}

//���Ժ���
bool CQ::EVEPrivateMsg::fromPrivate() const { return subType == 11; }

//��������״̬
bool CQ::EVEPrivateMsg::fromOnlineStatus() const { return subType == 1; }

//����Ⱥ��ʱ
bool CQ::EVEPrivateMsg::fromGroup() const { return subType == 2; }

//������������ʱ
bool CQ::EVEPrivateMsg::fromDiscuss() const { return subType == 3; }

msg CQ::EVEGroupMsg::sendMsg()const { return msg(fromGroup, msgtype::Ⱥ); }
msg CQ::EVEPrivateMsg::sendMsg() const { return msg(fromQQ, msgtype::����); }
msg CQ::EVEDiscussMsg::sendMsg() const { return msg(fromQQ, msgtype::������); }

int CQ::EVEPrivateMsg::sendMsg(const char * msg) const { return sendPrivateMsg(fromQQ,msg); }
int CQ::EVEPrivateMsg::sendMsg(std::string msg) const { return sendPrivateMsg(fromQQ,msg); }
int CQ::EVEGroupMsg::sendMsg(const char * msg) const { return sendGroupMsg(fromGroup, msg); }
int CQ::EVEGroupMsg::sendMsg(std::string msg) const { return sendGroupMsg(fromGroup, msg); }
int CQ::EVEDiscussMsg::sendMsg(const char * msg) const { return sendDiscussMsg(fromDiscuss, msg); }
int CQ::EVEDiscussMsg::sendMsg(std::string msg) const { return sendDiscussMsg(fromDiscuss, msg); }

CQ::EVEDiscussMsg::EVEDiscussMsg(int subType, int msgId, long long fromDiscuss, long long fromQQ, const char * msg, int font)
	:EVEMsg(subType, msgId, fromQQ, msg, font), fromDiscuss(fromDiscuss) {}

bool CQ::EVEDiscussMsg::leave() const { return !CQ::setDiscussLeave(fromDiscuss); }

void CQ::EVEStatus::color_green() { color = 1; }
void CQ::EVEStatus::color_orange() { color = 2; }
void CQ::EVEStatus::color_red() { color = 3; }
void CQ::EVEStatus::color_crimson() { color = 4; }
void CQ::EVEStatus::color_black() { color = 5; }
void CQ::EVEStatus::color_gray() { color = 6; }

std::string CQ::statusEVEreturn(EVEStatus & eve)
{
	Unpack pack;
	std::string _ret = pack.add(eve.data).add(eve.dataf).add(eve.color).getAll();
	_ret = base64_encode(_ret);
	return _ret;
}

CQ::EVERequest::EVERequest(int sendTime, long long fromQQ, const char * msg, const char * responseFlag)
	: sendTime(sendTime), fromQQ(fromQQ), msg(msg), responseFlag(responseFlag)
{}

CQ::EVERequestAddFriend::EVERequestAddFriend(int subType, int sendTime, long long fromGroup, long long fromQQ, const char * msg, const char * responseFlag)
	: EVERequest(sendTime, fromQQ, msg, responseFlag), subType(subType)
{}

void CQ::EVERequestAddFriend::pass(std::string msg)
{
	setFriendAddRequest(responseFlag, ����_ͨ��, msg.c_str());
}
void CQ::EVERequestAddFriend::fail(std::string msg)
{
	setFriendAddRequest(responseFlag, ����_�ܾ�, msg.c_str());
}
CQ::EVERequestAddGroup::EVERequestAddGroup(int subType, int sendTime, long long fromGroup, long long fromQQ, const char * msg, const char * responseFlag)
	: EVERequest(sendTime, fromQQ, msg, responseFlag), subType(subType), fromGroup(fromGroup)
{}

void CQ::EVERequestAddGroup::pass(std::string msg)
{
	setGroupAddRequest(responseFlag, subType, ����_ͨ��, msg.c_str());
}

void CQ::EVERequestAddGroup::fail(std::string msg)
{
	setGroupAddRequest(responseFlag, subType, ����_�ܾ�, msg.c_str());
}

AnonymousInfo::AnonymousInfo(const char* msg)
{
	if (msg[0] == '\0')
	{
		AID = 0; ���� = "";
	}
	else {
		Unpack p(base64_decode(msg));
		AID = p.getLong();
		���� = p.getstring();
		//Token = p.getchars();
	}
}
CQ::regexMsg::regexMsg(std::string msg)
{
	Unpack msgs(base64_decode(msg));
	auto len = msgs.getInt();//��ȡ��������
	while (len-- > 0) {
		auto tep = msgs.getUnpack();
		auto key = tep.getstring();
		auto value = tep.getstring();
		if (key == "") {
			return;
		}
		regexMap[key] = value;
	}
}
std::string CQ::regexMsg::get(std::string key)
{
	return regexMap[key];
}
std::string CQ::regexMsg::operator[](std::string key)
{
	return regexMap[key];
}