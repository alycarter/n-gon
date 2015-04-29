#include "ParticleEmmiter.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Time.h"


ParticleEmmiter::ParticleEmmiter(GraphicsManager * graphics, unsigned int particleCount) : LineRenderer(graphics)
{
	//add dead particles
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
	//clear the lines
	clear();
	//for every particle
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		//if the particle is alive
		if (particles.at(i).life > 0)
		{
			//lower its life
			particles.at(i).life -= package->time->getDeltaTime();
			//create the line from the particle
			LineInstance line;
			line.color = XMFLOAT4(1.0f,0.5f,0.2f,1);
			line.n1 = particles.at(i).normal;
			line.n2 = particles.at(i).normal;
			//load the particles velocity
			XMVECTOR vel = XMLoadFloat3(&particles.at(i).velocity);
			//move the partlicce along
			XMStoreFloat3(&particles.at(i).position, XMLoadFloat3(&particles.at(i).position) + (XMLoadFloat3(&particles.at(i).velocity) * package->time->getDeltaTime()));
			//the the particle a=had reaches 15 units long
			if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&particles.at(i).position) - XMLoadFloat3(&particles.at(i).endPosition))) > 15)
			{
				//move the end of the particle trail aswell
				XMStoreFloat3(&particles.at(i).endPosition, XMLoadFloat3(&particles.at(i).endPosition) + (XMLoadFloat3(&particles.at(i).velocity) * package->time->getDeltaTime()));
			}
			//store the line points
			line.p2 = particles.at(i).position;
			line.p1 = particles.at(i).endPosition;
			XMStoreFloat4x4(&line.model, XMMatrixIdentity());
			//add the line
			addLine(line);
		}
	}
}

void ParticleEmmiter::emmitParticle(XMVECTOR * position, XMVECTOR * velocity)
{
	bool found = false;
	//while we havnt reached the end particle or found a dead particle
	for (unsigned int i = 0; i < particles.size() && !found; i++)
	{
		//if the particle is dead
		if (particles.at(i).life <= 0)
		{
			//we found a particle
			found = true;
			//store the new particle
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