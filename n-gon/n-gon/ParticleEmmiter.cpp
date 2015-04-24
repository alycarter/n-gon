#include "ParticleEmmiter.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Time.h"


ParticleEmmiter::ParticleEmmiter(GraphicsManager * graphics, unsigned int particleCount) : LineRenderer(graphics)
{
	for (unsigned int i = 0; i < particleCount; i++)
	{
		particles.push_back(Particle());
		particles.at(i).life = 0;
	}
}


ParticleEmmiter::~ParticleEmmiter()
{
}

void ParticleEmmiter::update(UpdatePackage * package)
{
	clear();
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		if (particles.at(i).life > 0)
		{
			particles.at(i).life -= package->time->getDeltaTime();
			LineInstance line;
			line.color = XMFLOAT4(1.0f,0.5f,0.2f,1);
			line.n1 = particles.at(i).normal;
			line.n2 = particles.at(i).normal;
			XMVECTOR vel = XMLoadFloat3(&particles.at(i).velocity);
			XMStoreFloat3(&particles.at(i).position, XMLoadFloat3(&particles.at(i).position) + (XMLoadFloat3(&particles.at(i).velocity) * package->time->getDeltaTime()));
			if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&particles.at(i).position) - XMLoadFloat3(&particles.at(i).endPosition))) > 15)
			{
				XMStoreFloat3(&particles.at(i).endPosition, XMLoadFloat3(&particles.at(i).endPosition) + (XMLoadFloat3(&particles.at(i).velocity) * package->time->getDeltaTime()));
			}
			line.p2 = particles.at(i).position;
			line.p1 = particles.at(i).endPosition;
			XMStoreFloat4x4(&line.model, XMMatrixIdentity());
			addLine(line);
		}
	}
}

void ParticleEmmiter::emmitParticle(XMVECTOR * position, XMVECTOR * velocity)
{
	bool found = false;
	for (unsigned int i = 0; i < particles.size() && !found; i++)
	{
		if (particles.at(i).life <= 0)
		{
			found = true;
			XMStoreFloat3(&particles.at(i).position, *position);
			XMStoreFloat3(&particles.at(i).endPosition, *position);
			XMStoreFloat3(&particles.at(i).velocity, *velocity);
			XMVECTOR normal;
			calculateLineNormal(&normal, &XMFLOAT3(0,0,0), &particles.at(i).velocity);
			XMStoreFloat3(&particles.at(i).normal, normal * 2.5f);
			particles.at(i).life = 0.25f;
		}
	}
}