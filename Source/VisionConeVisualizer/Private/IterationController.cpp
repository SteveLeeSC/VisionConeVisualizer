// Fill out your copyright notice in the Description page of Project Settings.


#include "IterationController.h"

FIterationController::FIterationController()
{
}

FIterationController::~FIterationController()
{
}

bool FIterationController::Init()
{
	return FRunnable::Init();
}

uint32 FIterationController::Run()
{
	return 0;
}

void FIterationController::Exit()
{
	FRunnable::Exit();
}

void FIterationController::Stop()
{
	FRunnable::Stop();
}
