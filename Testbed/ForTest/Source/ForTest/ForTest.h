// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ForTest, Log, All);

#define LOG_CORE (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LOG_SIMPLE(Verbosity) UE_LOG(ForTest, Verbosity, TEXT("%s"), *LOG_CORE)
#define LOG_APPEND(Verbosity, Format, ...) UE_LOG(ForTest, Verbosity, TEXT("%s %s"),  *LOG_CORE, *FString::Printf(Format, ##__VA_ARGS__))

