<script lang="ts">
	import { start, end } from '../store/store';
	import { goto } from '$app/navigation';
	import { onMount } from 'svelte';
	import ImageSelector from '../lib/ImageSelector.svelte';
	import logo from '$lib/assets/wiggleLogo.svg';

	let openImageSelector: Boolean | 'start' | 'end' = false;
	let startTime = '';
	let endTime = '';

	function close() {
		openImageSelector = false;
	}

	function scheduleStart() {
		openImageSelector = 'start';
	}
	function scheduleEnd() {
		openImageSelector = 'end';
	}

	function imageSelected(e: CustomEvent) {
		if (openImageSelector === 'start') {
			start.update((start) => (start = e.detail));
		}
		if (openImageSelector === 'end') {
			end.update((end) => (end = e.detail));
		}
		openImageSelector = false;
	}

	onMount(async () => {
		await import('@kor-ui/kor');
		await import('@kor-ui/kor/kor-styles.css');
	});

	start.subscribe((start) => (startTime = start));
	end.subscribe((end) => (endTime = end));
</script>

<div class="wiggle-logo">
	<img src={logo} height="100%" alt="Wiggle Logo" width="40px">
</div>

<div class="actionsBar">
	<kor-button class="double-button left" label={"24 hours"} on:click={scheduleStart} />
	<!-- <kor-button class="double-button left" icon="schedule" label={startTime || 'Start'} on:click={scheduleStart} /> -->
	<!-- <kor-button class="double-button right" icon="schedule" label={endTime || 'End'} on:click={scheduleEnd} /> -->
</div>
{#if openImageSelector}
	<ImageSelector on:select={imageSelected} on:close={close} />
{/if}

<slot></slot>

<style>
	.wiggle-logo {
		position: fixed;
		top: 20px;
		left: 20px;
		padding: 20px;
		padding-bottom: 10px;
		padding-right: 25px;
		background-color: var(--main-color-darker);
		border: 1px solid white;
	}
	.wiggle-logo:hover {
		background-color: #4F4511;
	}
	.actionsBar {
		display: flex;
		position: fixed;
		right: 30px;
		top: 20px;
		z-index: 30;
	}
	.double-button {
		color: #201566;
		border-radius: 0;
		background-color: white;
		border: 3px solid var(--main-color-darker);
	}
	.double-button:hover {
		background-color: #ffffff;
		color: black;
	}
</style>
