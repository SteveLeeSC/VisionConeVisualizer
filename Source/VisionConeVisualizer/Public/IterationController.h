// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VISIONCONEVISUALIZER_API FIterationController : FRunnable
{
public:
	FIterationController();
	virtual ~FIterationController() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;
};
