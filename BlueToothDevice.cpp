#include "BlueToothDevice.h"

BlueToothDevice * BlueToothDevice::Instance()
{
	if (!m_pInstance.get()) {
		m_pInstance = std::shared_ptr<BlueToothDevice>(new BlueToothDevice());
	}
	else {
		return m_pInstance.get();
	}
}

BlueToothDevice::BlueToothDevice()
{
	cw.
}
