#pragma once
#include "LineRenderer.h"

struct Particle
{
	XMFLOAT3 position;
	XMFLOAT3 endPosition;
	XMFLOAT3 normal;
	XMFLOAT3 velocity;
	float life;
};

class ParticleEmmiter :
	public LineRenderer
{
public:
	ParticleEmmiter(GraphicsManager * graphics, unsigned int particleCount);
	~ParticleEmmiter();
	void update(UpdatePackage * package);
	void emmitParticle(XMVECTOR * position, XMVECTOR * velocity);
private:
	vector<Particle> particles;
};

