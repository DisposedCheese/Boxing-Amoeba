# include "AI.h"

AI::AI(double px, double py, double radius,double scale, Amoeba* player, bool normal)
{
	velX = 0;
	velY = 0;

	this->player = player;
	this->px = px;
	this->py = py;
	this->radius = radius*scale;
	this->normal = normal;
	this->scale = scale;

	needToResize = false;

	isHit  = false;
	isHitTimer = 0;

	attackArm = NULL;

	attackActive = false;
	attackArmTimer = 0;
	attackSpacing1 = 0;
	attackSpacing2 = 0;
	attackSpacing3 = 0;

	lslope = 0;

	defendArm = NULL;

	defendActive = false;
	defendSegActive = false;
	defendSeg2Active = false;
	defendArmTimer = 0;

	defendSpacing1 = 0;
	defendSpacing2 = 0;
	defendSpacing3 = 0;

	rslope = 0;


	isCollision = false;
	colPx = 0;
    colPy = 0;
	colAngle = 0;


	isBody = false;
	isDefend = false;
	isAttack = false;
	isWall = false;

	balls.addMetaball(new Metaball2D(this->px,this->py,this->radius));//body of AI
}

void AI::update()
{

	double distance = sqrt(  ((py - player->getPy()) * (py - player->getPy()) ) +  ((px - player->getPx() ) * (px - player->getPx() ))) ; 

	if(distance < radius + player->getRadius() + 50)//Approximate arm distance needed to attack 
	{
		int x = rand()%2;

		if(x == 0)
		{
			extendAttackArm();
		}
		else
		{
			extendDefendArm();
		}

	}

	if(!isCollision)
	{

		if(player->getPx() > px)
		{
			if( velX < 2)
				velX += 0.2;
		}
		else
		{
			if( velX > -2)
				velX += -0.2;
		}

		if(player->getPy() > py)
		{
			if( velY < 2)
				velY += 0.2;
		}
		else
		{
			if( velY > -2)
				velY += -0.2;
		}
	}
	else if(isBody)
	{
		if(player->getPx() > px)
		{
			if( velX < 2)
				velX += -0.5;
		}
		else
		{
			if( velX > -2)
				velX += 0.5;
		}

		if(player->getPy() > py)
		{
			if( velY < 2)
				velY += -0.5;
		}
		else
		{
			if( velY > -2)
				velY += 0.5;
		}

	}

	

	Amoeba::update();
			
};


void AI::extendAttackArm()
{
	if(lslope == 0)
	{

		lslope = ( ( player->getPy() - py) / (player->getPx() - px) );
		double angle = atan(lslope);

		attackSpacing1 = radius + radius/2;
		attackSpacing2 = attackSpacing1 + radius/2;
		attackSpacing3 = attackSpacing2 + radius/2;


		if(player->getPx() < px)
		{

			attackSpacing1 = (-1)*attackSpacing1;
			attackSpacing2 = (-1)*attackSpacing2;
			attackSpacing3 = (-1)*attackSpacing3;
		}



		if(!attackActive)
		{
			attackActive = true;
			attackArmTimer = time(NULL);

			attackArm = new Metaball2DGroup(1.0, 0.0, 0.0);
			attackArm->addMetaball(new Metaball2D(px + cos(angle)*attackSpacing1, py + sin(angle)*attackSpacing1, scale*3.0));
			attackArm->addMetaball(new Metaball2D(px + cos(angle) *attackSpacing2, py + sin(angle)*attackSpacing2,scale*3.0));
			attackArm->addMetaball(new Metaball2D(px + cos(angle)* attackSpacing3, py + sin(angle)*attackSpacing3,scale*3.0));
			balls.addSubgroup(attackArm);

			attackFistPx = px + cos(angle)*attackSpacing3;
			attackFistPy = py + sin(angle)*attackSpacing3;
			attackFistRadius = scale*3;

		}	
	}
}

void AI::extendDefendArm()
{
	if(rslope == 0)
	{

		rslope =( ( player->getPy() - py) / (player->getPx() - px) );
		double angle = atan(rslope);

		
		defendSpacing1 = radius + radius/2;
		defendSpacing2 = defendSpacing1 + radius/2;
		defendSpacing3 = defendSpacing2 + radius/2;

		if(player->getPx() < px)
		{
			defendSpacing1 = (-1)*defendSpacing1;
			defendSpacing2 = (-1)*defendSpacing2;
			defendSpacing3 = (-1)*defendSpacing3;
		}
		

		if(!defendActive && (defendWaitTimer - time(NULL) <= 0 ))
		{
			defendActive = true;
			defendArmTimer = time(NULL);
			defendWaitTimer = time(NULL) + 7;
			defendArm = new Metaball2DGroup(0.0, 1.0, 0.0);

			defendArm->addMetaball(new Metaball2D(px + cos(angle)* defendSpacing1, py + sin(angle) *defendSpacing1, scale*3.0));
			defendArm->addMetaball(new Metaball2D(px + cos(angle)* defendSpacing2, py + sin(angle) *defendSpacing2,scale*3.0));
			defendArm->addMetaball(new Metaball2D(px + cos(angle)* defendSpacing3, py + sin(angle)*defendSpacing3,scale*12.0));
			balls.addSubgroup(defendArm);

			defendFistPx = px + cos(angle)*defendSpacing3;
			defendFistPy = py + sin(angle)*defendSpacing3;
			defendFistRadius = scale*12;
		}
		else
		{
			rslope = 0;
		}
	}


}

