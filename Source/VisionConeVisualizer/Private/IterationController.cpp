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

void FIterationController::Run()
{
}

void FIterationController::Exit()
{
	FRunnable::Exit();
}

void FIterationController::Stop()
{
	FRunnable::Stop();
}
