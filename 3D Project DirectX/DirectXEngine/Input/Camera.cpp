#include "Camera.h"
//
//
//
//Camera::Camera()
//{
//	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
//	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
//	this->CamPos = XMLoadFloat3(&this->pos);
//	this->rotVector = XMLoadFloat3(&this->rot);
//	SetProjectionValues(90.0f ,float(WIDTH / HEIGHT), 0.1f, 20.0f);
//	updateViewMatrix();
//}
//
//Camera::~Camera()
//{
//}
//
//void Camera::SetProjectionValues(float fovDegree, float apsectRadio, float nearz, float farZ)
//{
//
//	// XM_PI eller XM_2PI
//	float fovRadians = (fovDegree / 360.0f)* DirectX::XM_2PI; //360.0f)*XM_2PI;
//	this->projection = XMMatrixPerspectiveFovLH(fovRadians, apsectRadio, nearz, farZ);
//
//}
//
//const XMMATRIX & Camera::GetViewMatrix() const
//{
//	return this->view;
//}
//
//const XMMATRIX & Camera::GetProjectionMatrix() const
//{
//	return this->projection;
//}
//
//const XMVECTOR & Camera::GetPositionVector() const
//{
//	return this->CamPos;
//}
//
//const XMVECTOR & Camera::GetRoationFloat3() const
//{
//	return this->rotVector;
//}
//
//void Camera::SetPosition(const XMVECTOR & pos)
//{
//	//XMStoreFloat3(&this->Pos, pos);
//	this->CamPos = pos;
//	this->updateViewMatrix();
//}
//
//void Camera::AdjustPosition(const XMVECTOR & pos)
//{
//	this->CamPos += pos;
//	XMStoreFloat3(&this->pos, this->CamPos);
//	this->updateViewMatrix();
//}
//
//
//void Camera::SetRoation(const XMVECTOR & rot)
//{
//	this->rotVector = rot;
//	XMStoreFloat3(&this->rot, rot);
//	this->updateViewMatrix();
//}
//
//void Camera::AdjustRotiation(const XMVECTOR & rot)
//{
//	this->rotVector += rot;
//	XMStoreFloat3(&this->rot, this->rotVector);
//	this->updateViewMatrix();
//}
//
////to check object.
//void Camera::SetLookAtPos(XMFLOAT3 lookatPos)
//{
//	//check so the lookAtPos isnt the same as camPos, they can not be the same
//	if (lookatPos.x == pos.x && lookatPos.y == pos.y && lookatPos.z == pos.z) {
//		return;
//	}
//	//calc diffrense to  get diffrnse
//	lookatPos.x = pos.x - lookatPos.x;
//	lookatPos.y = pos.y - lookatPos.y;
//	lookatPos.z = pos.z - lookatPos.z;
//
//	float pitch = 0.0f;
//	if (lookatPos.y != 0.0f) {
//		const float distance = sqrt(lookatPos.x *lookatPos.x + lookatPos.z *lookatPos.z);
//		pitch = atan(lookatPos.y / distance);
//	}
//	float yaw = 0.0f;
//	if (lookatPos.x != 0.0f) {
//		yaw = atan(lookatPos.x / lookatPos.z);
//	}
//	if (lookatPos.z > 0) {
//		yaw += XM_PI;
//	}
//	//Roll not needed we dont wanna turn it like a airplane
//	XMFLOAT3 set = XMFLOAT3(pitch, yaw, 0.0f);
//	this->SetRoation(XMLoadFloat3(&set));
//
//	//to calculate yaw we only need x and z value
//}
//
//void Camera::updateViewMatrix()
//{//Calculate camera rotation matrix
//	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
//	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
//	XMVECTOR camTarget = XMVector3TransformCoord(this->forwardVector, camRotationMatrix);
//	//Adjust cam target to be offset by the camera's current position
//
//
//	camTarget += XMLoadFloat3(&pos);//this->PosVector;
//	//Calculate up direction based on current rotation
//	XMVECTOR upDir = XMVector3TransformCoord(this->upVector, camRotationMatrix);
//	//Rebuild view matrix
//	this->view = XMMatrixLookAtLH(this->CamPos, camTarget, upDir);
//}

Camera::Camera()
{
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
	SetProjectionValues(45.0f, float(HEIGHT / float(WIDTH)), 0.1f, 200.0f);
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 180.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
	ratio = aspectRatio;
}

const XMMATRIX & Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX & Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const XMVECTOR & Camera::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3 & Camera::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR & Camera::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3 & Camera::GetRotationFloat3() const
{
	return this->rot;
}

void Camera::SetPosition(const XMVECTOR & pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(const XMFLOAT3 & pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR & pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMFLOAT3 & pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR & rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMFLOAT3 & rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR & rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMFLOAT3 & rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR & Camera::GetForwardVector()
{
	return this->vec_forward;
}

const XMVECTOR & Camera::GetRightVector()
{
	return this->vec_right;
}

const XMVECTOR & Camera::GetBackwardVector()
{
	return this->vec_backward;
}

const XMVECTOR & Camera::GetLeftVector()
{
	return this->vec_left;
}

const float Camera::GetProjectionRatio()
{
	return ratio;
}

void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{
	//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += this->posVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	//Rebuild view matrix
	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}